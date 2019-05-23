#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <set>

#include <llvm/Support/FileSystem.h>
#include <llvm/Support/MemoryBuffer.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/Bitcode/BitcodeReader.h>
#include <llvm/IR/InstIterator.h>
#include <llvm/IR/Instruction.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/InstrTypes.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/DataLayout.h>
#include <llvm/ADT/Optional.h>
#include <llvm/ADT/StringRef.h>

#include "AUA/Alias/Pointer.h"

std::map<std::string, Pointer*> pointers;
std::map<std::string, VarRef*> vars;

std::unique_ptr<llvm::Module> readInModule(llvm::LLVMContext &context, std::string inFile);
void analyzeInstruction(llvm::Instruction *I, llvm::Instruction *previous);
void printPointerInfo();
void printVarInfo();
void printFullCurrentInfo();
void handleAllocation(llvm::AllocaInst *allocaInst);
void handleCopy(llvm::LoadInst* loadInst, llvm::StoreInst*);
void handleAssignment(llvm::StoreInst *storeInst);

int main(int argc, char **argv) {
    std::string inFile = argv[1];
    llvm::LLVMContext context;
    llvm::Module *module = readInModule(context,inFile).release();

    for (auto &F: module->getFunctionList()) {

        llvm::Instruction* previous;

        for (llvm::inst_iterator I = inst_begin(F), E = inst_end(F); I != E; ++I) {
            llvm::Instruction *inst = &*I;
            llvm::outs() << *inst << "\n";
            analyzeInstruction(inst, previous);
            previous = inst;
        }
    }

    printFullCurrentInfo();

    return 0;
}

void analyzeInstruction(llvm::Instruction * I, llvm::Instruction *previous) {

    if(llvm::AllocaInst *allocaInst = llvm::dyn_cast<llvm::AllocaInst>(I)) {
        handleAllocation(allocaInst);
    }

    if (llvm::StoreInst *storeInst = llvm::dyn_cast<llvm::StoreInst>(I)) {

        if(llvm::LoadInst *loadInst = llvm::dyn_cast<llvm::LoadInst>(previous)) {
            handleCopy(loadInst, storeInst);
        } else {
            handleAssignment(storeInst);
        }

        printPointerInfo();
    }
}

void printPointerInfo() {
    llvm::outs() << "\n" << pointers.size() << " Pointers:\n";
    for(std::pair<std::string, Pointer*> p : pointers) {
        llvm::outs() << p.first << " points to: ";
        for(VarRef* v : p.second->getVarRefs()) {
            llvm::outs() << v->getName() << " ";
        }
        llvm::outs() << "\n";
    }

    llvm::outs() << "\n";
}

void printVarInfo() {
    llvm::outs() << "\n" << vars.size() << " variables:\n";
    for(std::pair<std::string, VarRef*> v : vars) {
        llvm::outs() << v.first << "\n";
    }
    llvm::outs() << "\n";
}

void printFullCurrentInfo() {

    printPointerInfo();
    printVarInfo();

}

void handleAllocation(llvm::AllocaInst *allocaInst) {

    llvm::Type *type = allocaInst->getAllocatedType();
    std::string name = (allocaInst->getName());

    if (type->isPointerTy()) {

        Pointer* p = new Pointer(0, name);
        pointers.insert(std::pair<std::string, Pointer*> (name, p));

    } else {

        VarRef* v = new VarRef(0, name);
        vars.insert(std::pair<std::string, VarRef*> (name, v));

    }
}

void handleCopy(llvm::LoadInst* loadInst, llvm::StoreInst* storeInst) {

    assert(loadInst->getName().data() == storeInst->getValueOperand()->getName().data());

    llvm::PointerType* fromType = llvm::cast<llvm::PointerType>(loadInst->getPointerOperandType());
    llvm::PointerType* toType = llvm::cast<llvm::PointerType>(storeInst->getPointerOperandType());

    assert(fromType == toType);

    if(!fromType->getElementType()->isPointerTy()){return;}

    Pointer* copyFrom = pointers[loadInst->getPointerOperand()->getName()];
    Pointer* copyTo = pointers[storeInst->getPointerOperand()->getName()];

    copyTo->copyPointersFrom(copyFrom);

}

void handleAssignment(llvm::StoreInst *storeInst) {

    llvm::Value* value = storeInst->getValueOperand();
    llvm::Value* dest = storeInst->getPointerOperand();

    if(!value->getType()->isPointerTy()) {return;}

    llvm::PointerType* ptrType = llvm::cast<llvm::PointerType>(dest->getType());
    if(!ptrType->getElementType()->isPointerTy()){return;}

    VarRef* v = vars[value->getName()];

    pointers[dest->getName()]->onlyPointTo(v);
}

/**
* creates llvm module based on llvm context and the input file that was given while constructing the compile object
* @param llvmContext
* @return
*/
std::unique_ptr<llvm::Module> readInModule(llvm::LLVMContext &context, std::string inputFile) {
    std::cout << "getting module from bitcode for file " << inputFile << "\n";
    llvm::ErrorOr <std::unique_ptr<llvm::MemoryBuffer>> bufferOrError =
            llvm::MemoryBuffer::getFile(inputFile);
    if (std::error_code EC = bufferOrError.getError()) {
        std::cout  << "Cannot open bitcode file (errorCode " + EC.message() + ")";
    }
    std::unique_ptr <llvm::MemoryBuffer> buffer(std::move(*bufferOrError));
    llvm::MemoryBufferRef bitcodeBufferRef(buffer->getBuffer(), buffer->getBufferIdentifier());
    llvm::Expected <std::unique_ptr<llvm::Module>> expectedBcForSlicing =
            llvm::parseBitcodeFile(bitcodeBufferRef, context);
    if (!expectedBcForSlicing) {
        std::error_code ec = llvm::errorToErrorCode(expectedBcForSlicing.takeError());
        std::cout << "error reading bitcode from buffer ";
    }
    return std::move(expectedBcForSlicing.get());
}