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

#include "Reference.h"
#include "RefType.h"

std::set<Reference> refs;

std::unique_ptr<llvm::Module> readInModule(llvm::LLVMContext &context, std::string inFile);
void analyzeInstruction(llvm::Instruction *I);

int main(int argc, char **argv) {
    std::string inFile = argv[1];
    llvm::LLVMContext context;
    llvm::Module *module = readInModule(context,inFile).release();

    for (auto &F: module->getFunctionList()) {

        for (llvm::inst_iterator I = inst_begin(F), E = inst_end(F); I != E; ++I) {
            llvm::Instruction *inst = &*I;
            llvm::outs() << *inst << "\n";
            analyzeInstruction(inst);
        }
    }
    return 0;
}

void analyzeInstruction(llvm::Instruction * I) {

    if(llvm::AllocaInst *allocaInst = llvm::dyn_cast<llvm::AllocaInst>(I)) {

        llvm::Type *type = allocaInst->getAllocatedType();
        RefType refType;

        if (type->isPointerTy()) {
            llvm::outs() << (I->getName()) << " is a pointer!\n";
            refType = STACK_PTR;
        } else {
            llvm::outs() << (I->getName()) << " is not a pointer (local var)!\n";
            refType = LOCAL_VAR;
        }

        llvm::Optional<uint64_t> optSize = allocaInst->getAllocationSizeInBits(* new llvm::DataLayout(* new llvm::StringRef()));
        uint64_t size = (optSize.hasValue())? optSize.getValue() : 0;

        std::string name = (I->getName());

        Reference ref {size, refType, name};
    }

    if (llvm::isa<llvm::StoreInst>(I)) {
        llvm::StringRef destNameRef = I->getOperand(1)->getName();

        llvm::outs() << destNameRef.data() << "\n";
    }
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