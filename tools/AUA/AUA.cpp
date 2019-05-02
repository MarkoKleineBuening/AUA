#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>

#include <llvm/Support/FileSystem.h>
#include <llvm/Support/MemoryBuffer.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/Bitcode/BitcodeReader.h>
#include <llvm/IR/InstIterator.h>

std::unique_ptr<llvm::Module> readInModule(llvm::LLVMContext &context, std::string inFile);

int main(int argc, char **argv) {
    std::string inFile = argv[1];
    llvm::LLVMContext context;
    llvm::Module *module = readInModule(context,inFile).release();
    for (auto &F: module->getFunctionList()) {
        for (llvm::inst_iterator I = inst_begin(F), E = inst_end(F); I != E; ++I) {
            llvm::outs() << *I << "\n";
        }
    }
    return 0;
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