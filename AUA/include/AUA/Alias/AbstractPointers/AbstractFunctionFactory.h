//
// Created by mlaupichler on 10.07.19.
//

#ifndef AUA_ABSTRACTFUNCTIONFACTORY_H
#define AUA_ABSTRACTFUNCTIONFACTORY_H


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
#include <llvm/IR/Function.h>
#include <llvm/ADT/Optional.h>
#include <llvm/ADT/StringRef.h>
#include <llvm/IR/CFG.h>

#include <AUA/Alias/AbstractOps/PointerOperation.h>
#include <AUA/Alias/AbstractPointers/Finders/TargetFinder.h>
#include <AUA/Alias/AbstractPointers/Finders/PointerFinder.h>
#include <AUA/Alias/AbstractOps/ReturnOp.h>
#include <AUA/Alias/AbstractOps/DummyInitialOp.h>
#include <AUA/Alias/AbstractOps/JoinOp.h>
#include <AUA/Alias/AbstractPointers/Finders/CompositeFinder.h>
#include <AUA/Alias/AbstractPointers/Finders/FinderFactory.h>
#include "AbstractFunction.h"

class AbstractFunctionFactory {

private:

    llvm::DataLayout *dataLayout;
    FinderFactory *finderFactory;

    /**
    * Used to start the chain of PointerOperations. Is the only PointerOperation allowed to have no predecessors.
    */
    DummyInitialOp *initialOp = nullptr;

    /**
     * Used to collect dangling paths of PointerOperations into one final PointerOperation. Is the only PointerOperation allowed to have no successors.
     */
    ReturnOp *finalOp = nullptr;


    std::map<std::string, bool> loopLatchFound;
    std::map<std::string, JoinOp *> loopJoinOps;

    std::map<std::string, JoinOp *> ifJoinOps;

    /**
     * Resets state of abstracting functions including clearing found CFG joins and new initial and final Ops
     */
    void resetAbstractionState();

    /**
     * Abstracts a given BasicBlock into an ordered graph of PointerOperations.
     * Recursively calls abstractBasicBlock for all succeeding BasicBlocks to complete the ordered graph.
     * @param BB the BasicBlock to abstract
     * @return The first PointerOperation of this block if any are abstracted from it or the result of its following block else.
     */
    PointerOperation *abstractBasicBlock(llvm::BasicBlock *BB);

    PointerOperation *abstractLoopConditionBlock(llvm::BasicBlock *BB);

    PointerOperation *abstractIfRejoinBlock(llvm::BasicBlock *BB);

    PointerOperation *abstractInstruction(llvm::Instruction *I);

    PointerOperation *handleAllocation(llvm::AllocaInst *allocaInst);

    PointerOperation *handleStore(llvm::StoreInst *storeInst);

//    PointerOperation *handleAssignment(llvm::StoreInst *storeInst);

    PointerOperation *handleCallWithIrrelevantReturn(llvm::CallInst *callInst);

    ReturnOp *handleReturn(llvm::ReturnInst *returnInst);

    ReturnOp * buildFinalOp(llvm::ReturnInst *returnInst);

    PointerOperation *abstractIfBranchBlock(llvm::BasicBlock *BB);

    std::pair<PointerOperation *, PointerOperation *> *abstractBlockInstructions(llvm::BasicBlock *BB);

    static std::vector<std::string> getParamNames(llvm::Function *function);

    std::map<int, PointerFormat> * getPointerParamFormats(llvm::Function *function);

    std::map<int, CompositeFormat> * getCompositeParamFormats(llvm::Function *function);

    std::set<AbstractVar*>* getVarParams(llvm::Function *function);

public:

    AbstractFunctionFactory(llvm::DataLayout *dataLayout, FinderFactory *finderFactory);

    AbstractFunction *buildAbstractFunction(llvm::Function *function);
};

struct MultipleReturnException : public std::exception {
    const char *what() const throw() {
        return "More than one Instruction of type llvm::ReturnInst have been found.";
    }
};


#endif //AUA_ABSTRACTFUNCTIONFACTORY_H
