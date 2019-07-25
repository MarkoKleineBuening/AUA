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
#include <llvm/IR/CFG.h>

#include <llvm/Analysis/LoopInfo.h>

#include "PointerIrrelevantException.h"

#include <AUA/Alias/AbstractPointers/AbstractPointer.h>
#include <AUA/Alias/AbstractOps/AssignmentOp.h>
#include <AUA/Alias/AbstractPointers/Configuration.h>
#include <AUA/Alias/AbstractOps/PointerAllocationOp.h>
#include <AUA/Alias/AbstractOps/VarAllocationOp.h>
#include <AUA/Alias/AbstractOps/CopyOp.h>
#include <AUA/Alias/AbstractOps/ReturnOp.h>
#include <AUA/Alias/AbstractOps/JoinOp.h>
#include <AUA/Alias/AbstractOps/SplitOp.h>
#include <AUA/Alias/AbstractOps/DummyInitialOp.h>
#include <AUA/Alias/AbstractOps/CompositeAllocationOp.h>
#include <AUA/Alias/AbstractPointers/Finders/MemberPointerFinder.h>
#include <AUA/Alias/AbstractPointers/Finders/BasePointerFinder.h>
#include <AUA/Alias/AbstractPointers/Finders/MemberTargetFinder.h>
#include <AUA/Alias/AbstractPointers/Finders/BaseTargetFinder.h>
#include <AUA/Alias/AbstractPointers/AbstractFunctionFactory.h>
#include <AUA/Alias/AbstractPointers/GlobalConfiguration.h>


AbstractFunctionFactory *funcFac;

std::unique_ptr<llvm::Module> readInModule(llvm::LLVMContext &context, std::string inFile);

GlobalConfiguration* Configuration::global = new GlobalConfiguration();
int AnonymousPointerFinder::anonPointerCounter = 0;

int main(int argc, char **argv) {

    std::string inFile = argv[1];
    llvm::LLVMContext context;
    llvm::Module *module = readInModule(context, inFile).release();

    auto dl = new llvm::DataLayout(module);
    auto finderFactory = new FinderFactory(dl);
    funcFac = new AbstractFunctionFactory(dl, finderFactory);

    for (auto &F: module->getFunctionList()) {

        AbstractFunction *function = funcFac->buildAbstractFunction(&F);
        Configuration::global->addGlobalFunction(function->getName(), function);

    }

    auto mainMethod = Configuration::global->getGlobalFunction("main");

    auto pointerParams = std::map<int, PointerSetValue*>();
    auto compositeParams = std::map<int, CompositeSetValue*>();
    mainMethod->execute(pointerParams, compositeParams);

    return 0;
}


/**
* creates llvm module based on llvm context and the input file that was given while constructing the compile object
* @param llvmContext
* @return
*/
std::unique_ptr<llvm::Module> readInModule(llvm::LLVMContext &context, std::string inputFile) {
    std::cout << "getting module from bitcode for file " << inputFile << "\n";
    llvm::ErrorOr<std::unique_ptr<llvm::MemoryBuffer>> bufferOrError =
            llvm::MemoryBuffer::getFile(inputFile);
    if (std::error_code EC = bufferOrError.getError()) {
        std::cout << "Cannot open bitcode file (errorCode " + EC.message() + ")";
    }
    std::unique_ptr<llvm::MemoryBuffer> buffer(std::move(*bufferOrError));
    llvm::MemoryBufferRef bitcodeBufferRef(buffer->getBuffer(), buffer->getBufferIdentifier());
    llvm::Expected<std::unique_ptr<llvm::Module>> expectedBcForSlicing =
            llvm::parseBitcodeFile(bitcodeBufferRef, context);
    if (!expectedBcForSlicing) {
        std::error_code ec = llvm::errorToErrorCode(expectedBcForSlicing.takeError());
        std::cout << "error reading bitcode from buffer ";
    }
    return std::move(expectedBcForSlicing.get());
}