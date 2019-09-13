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
#include <llvm/IR/Dominators.h>

#include <llvm/Analysis/LoopInfo.h>

#include <AUA/Alias/AbstractOps/PointerOperation.h>
#include <AUA/Alias/AbstractPointers/Finders/TargetFinder.h>
#include <AUA/Alias/AbstractPointers/Finders/PointerFinder.h>
#include <AUA/Alias/AbstractOps/ReturnOp.h>
#include <AUA/Alias/AbstractOps/DummyInitialOp.h>
#include <AUA/Alias/AbstractOps/JoinOp.h>
#include <AUA/Alias/AbstractPointers/Finders/CompositeFinder.h>
#include <AUA/Alias/AbstractPointers/Finders/FinderFactory.h>
#include <AUA/Alias/AbstractPointers/AbstractFunction.h>
#include <AUA/Alias/AbstractPointers/GlobalConfiguration.h>
#include <AUA/Alias/AbstractPointers/AbstractLoop.h>


struct BlockAbstractionState {

    /**
     * Specifies if this block and its successors have been fully abstracted.
     */
    bool done;

    /**
     * The first PointerOperation that is reachable from the beginning of this block.
     */
    PointerOperation* firstReachable;

    /**
     * The last PointerOperation that is reachable from the beginning of this block. Has to be the ReturnOp of the function.
     */
    ReturnOp* lastReachable;


    BlockAbstractionState() : firstReachable(nullptr), lastReachable(nullptr), done(false) {}

};


class AbstractFunctionFactory {

private:

    llvm::DataLayout *dataLayout;
    FinderFactory *finderFactory;



    /**
     * Abstracts a given BasicBlock into an ordered graph of PointerOperations.
     * Recursively calls abstractBasicBlock for all succeeding BasicBlocks to complete the ordered graph.
     * @param BB the BasicBlock to abstract
     * @return The first PointerOperation of this block if any are abstracted from it or the result of its following block else.
     */
    void abstractBasicBlock(llvm::BasicBlock *BB, std::map<llvm::BasicBlock *, BlockAbstractionState *> *state,
                            LoopBlockInfo *loopBlockInfo);


    PointerOperation *abstractInstruction(llvm::Instruction *I);

    PointerOperation *handleAllocation(llvm::AllocaInst *allocaInst);

    PointerOperation *handleStore(llvm::StoreInst *storeInst);

    PointerOperation *handleCallWithIrrelevantReturn(llvm::CallInst *callInst);

    ReturnOp *handleReturn(llvm::ReturnInst *returnInst);

    std::pair<PointerOperation *, PointerOperation *> *abstractBlockInstructions(llvm::BasicBlock *BB);

    static std::vector<std::string> getParamNames(llvm::Function *function);

    std::map<int, PointerFormat> * getPointerParamFormats(llvm::Function *function);

    std::map<int, CompositeFormat> * getCompositeParamFormats(llvm::Function *function);

    std::set<AbstractVar*>* getVarParams(llvm::Function *function);

    LoopBlockInfo* getTopLevelLoopBlockInfo(llvm::Function* function);

public:

    int totalInstructionCount = 0;
    int totalPointerCount = 0;
    int totalCallCount = 0;

    AbstractFunctionFactory(llvm::DataLayout *dataLayout, FinderFactory *finderFactory);


    /**
     * Constructs new AbstractFunction for the given llvm::Function.
     * @param function an llvm::Function object to abstract.
     * @return the abstracted function.
     */
    AbstractFunction *buildAbstractFunction(llvm::Function *function);
};

struct MultipleReturnException : public std::exception {
    const char *what() const throw() {
        return "More than one Instruction of type llvm::ReturnInst have been found.";
    }
};


#endif //AUA_ABSTRACTFUNCTIONFACTORY_H
