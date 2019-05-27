#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <set>
#include <list>

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

#include "PointerIrrelevantException.h"

#include <AUA/Alias/AbstractPointers/AbstractPointer.h>
#include <AUA/Alias/AbstractOps/AssignmentOp.h>
#include <AUA/Alias/Configuration.h>
#include <AUA/Alias/AbstractOps/PointerAllocationOp.h>
#include <AUA/Alias/AbstractOps/VarAllocationOp.h>
#include <AUA/Alias/AbstractOps/CopyOp.h>


Configuration configuration;

std::list<PointerOperation*> opList;

std::unique_ptr<llvm::Module> readInModule(llvm::LLVMContext &context, std::string inFile);
PointerOperation* abstractInstruction(llvm::Instruction *I, llvm::Instruction *previous);
PointerOperation* handleAllocation(llvm::AllocaInst *allocaInst);
PointerOperation* handleCopy(llvm::LoadInst* loadInst, llvm::StoreInst*);
PointerOperation* handleAssignment(llvm::StoreInst *storeInst);

int main(int argc, char **argv) {
    std::string inFile = argv[1];
    llvm::LLVMContext context;
    llvm::Module *module = readInModule(context,inFile).release();

    configuration = Configuration();

    opList.clear();

    for (auto &F: module->getFunctionList()) {

        llvm::Instruction* previous;

        for (llvm::inst_iterator I = inst_begin(F), E = inst_end(F); I != E; ++I) {
            llvm::Instruction *inst = &*I;
            llvm::outs() << *inst << "\n";

            try {
                opList.push_back(abstractInstruction(inst, previous));
            } catch (PointerIrrelevantException & e) {
                // instruction wasn't relevant for pointers. Continue with next instruction.
            }

            previous = inst;
        }
    }

    std::cout << "\n" << "Pointer Operation count:" << opList.size() << "\n";

    for (auto it = opList.cbegin(); it != opList.cend(); it++) {

        configuration.printPointerInfo();
        llvm::outs() << "\n";

        configuration = (*it)->execute(configuration);

    }

    configuration.printFullInfo();

    return 0;
}

PointerOperation* abstractInstruction(llvm::Instruction *I, llvm::Instruction *previous) {


        if(llvm::AllocaInst *allocaInst = llvm::dyn_cast<llvm::AllocaInst>(I)) {
            return handleAllocation(allocaInst);
        }

        if (llvm::StoreInst *storeInst = llvm::dyn_cast<llvm::StoreInst>(I)) {

            if(llvm::LoadInst *loadInst = llvm::dyn_cast<llvm::LoadInst>(previous)) {
                return handleCopy(loadInst, storeInst);
            } else {
                return handleAssignment(storeInst);
            }

        }

        throw PointerIrrelevantException();

}

PointerOperation* handleAllocation(llvm::AllocaInst *allocaInst) {

    llvm::Type *type = allocaInst->getAllocatedType();
    std::string name = (allocaInst->getName());

    PointerOperation* allocOp;

    if (type->isPointerTy()) {

        allocOp = new PointerAllocationOp(name);

    } else {

        int size = (int) ceil(((double) allocaInst->getAllocationSizeInBits(llvm::DataLayout(llvm::StringRef())).getValue())/8.0);

        allocOp = new VarAllocationOp(name, size);

    }

    return allocOp;
}

/**
 * handles copy instructions. Throws PointerIrrelevantException.
 * @param loadInst the load instruction of the copy
 * @param storeInst the store instruction of the copy
 * @return the resulting Copy Pointer Operation
 */
PointerOperation* handleCopy(llvm::LoadInst* loadInst, llvm::StoreInst* storeInst) {

    assert(loadInst->getName().data() == storeInst->getValueOperand()->getName().data());

    llvm::PointerType* fromType = llvm::cast<llvm::PointerType>(loadInst->getPointerOperandType());
    llvm::PointerType* toType = llvm::cast<llvm::PointerType>(storeInst->getPointerOperandType());

    assert(fromType == toType);

    if(!fromType->getElementType()->isPointerTy()){throw PointerIrrelevantException();}

    std::string fromName = loadInst->getPointerOperand()->getName();
    std::string toName = storeInst->getPointerOperand()->getName();

    CopyOp* copyOp = new CopyOp(fromName, toName);

    return copyOp;
}

PointerOperation* handleAssignment(llvm::StoreInst *storeInst) {

    llvm::Value* value = storeInst->getValueOperand();
    llvm::Value* dest = storeInst->getPointerOperand();

    if(!value->getType()->isPointerTy()) {throw PointerIrrelevantException();}

    llvm::PointerType* ptrType = llvm::cast<llvm::PointerType>(dest->getType());
    if(!ptrType->getElementType()->isPointerTy()){throw PointerIrrelevantException();}

    AssignmentOp* assignOp = new AssignmentOp(dest->getName(), value->getName());

    return assignOp;
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