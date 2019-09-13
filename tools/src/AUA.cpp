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
#include <llvm/IR/Dominators.h>
#include <llvm/ADT/Optional.h>
#include <llvm/ADT/StringRef.h>
#include <llvm/IR/CFG.h>

#include <llvm/Analysis/LoopInfo.h>

#include <AUA/Alias/AbstractPointers/AbstractPointer.h>
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


std::unique_ptr<llvm::Module> readInModule(llvm::LLVMContext &context, std::string inputFile);


int main(int argc, char **argv) {

    std::string inFile = argv[1];
    llvm::LLVMContext context;
    llvm::Module *module = readInModule(context, inFile).release();


    auto global = new GlobalConfiguration(new GlobalValueFactory(module));
    Configuration::setGlobalConfiguration(global);

    std::string entryFunctionName = argv[2];

    auto dl = new llvm::DataLayout(module);
    auto functionFactory = new AbstractFunctionFactory(dl, new FinderFactory(dl));

    auto entryFunction = functionFactory->buildAbstractFunction(module->getFunction(entryFunctionName));

    llvm::outs() << entryFunction->getName() << "\n";

    if (!Configuration::global->hasGlobalFunction(entryFunction->getName())) {
        Configuration::global->addGlobalFunction(entryFunction);
    }

    entryFunction->executeAsEntry();

    llvm::outs() << "\nNumber of LLVM instructions: " << functionFactory->totalInstructionCount << "\n";
    llvm::outs() << "Number of pointers: " << functionFactory->totalPointerCount<< "\n";
    llvm::outs() << "Number of calls: " << functionFactory->totalCallCount << "\n";

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