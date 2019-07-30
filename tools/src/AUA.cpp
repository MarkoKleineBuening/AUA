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
#include <AUA/Alias/AbstractOps/StoreOp.h>
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
llvm::DataLayout* dl;

std::unique_ptr<llvm::Module> readInModule(llvm::LLVMContext &context, std::string inFile);

GlobalConfiguration* Configuration::global = new GlobalConfiguration();
int AnonymousPointerFinder::anonPointerCounter = 0;

void buildGlobals(llvm::Module* module);

int main(int argc, char **argv) {

    std::string inFile = argv[1];
    llvm::LLVMContext context;
    llvm::Module *module = readInModule(context, inFile).release();

    dl = new llvm::DataLayout(module);
    auto finderFactory = new FinderFactory(dl);
    funcFac = new AbstractFunctionFactory(dl, finderFactory);

    buildGlobals(module);

    for (auto &F: module->getFunctionList()) {

        AbstractFunction *function = funcFac->buildAbstractFunction(&F);
        Configuration::global->addGlobalFunction(function->getName(), function);

    }

    std::string entryFunctionName = argv[2];

    auto entryFunction = Configuration::global->getGlobalFunction(entryFunctionName);

    entryFunction->executeAsEntry();

    return 0;
}

void buildGlobals(llvm::Module* module) {

    ReferenceFlags globalFlags = ReferenceFlags(true, false, false);

    for (auto &global : module->getGlobalList()) {

        llvm::Type* type = global.getValueType();
        std::string name = global.getName();

        if (auto globalPtrType = llvm::dyn_cast<llvm::PointerType>(type)) {

            AbstractPointer* globalPtr = new AbstractPointer(name, PointerFormat(globalPtrType), globalFlags);
            Configuration::global->addGlobalPointer(globalPtr->getName(), globalPtr);

        } else if (auto globalCompType = llvm::dyn_cast<llvm::CompositeType>(type)) {

            AbstractComposite* globalComp = new AbstractComposite(name, CompositeFormat(globalCompType, dl), globalFlags);
            Configuration::global->addGlobalComposite(globalComp->getName(), globalComp);

        } else {

            AbstractVar* globalVar = new AbstractVar(name, dl->getTypeAllocSize(type), globalFlags);
            Configuration::global->addGlobalVar(globalVar->getName(), globalVar);

        }

    }

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