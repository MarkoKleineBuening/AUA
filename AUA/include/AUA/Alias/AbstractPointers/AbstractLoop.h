//
// Created by mlaupichler on 06.08.19.
//

#ifndef AUA_ABSTRACTLOOP_H
#define AUA_ABSTRACTLOOP_H


#include <llvm/IR/BasicBlock.h>
#include <llvm/Analysis/LoopInfo.h>
#include <AUA/Alias/AbstractOps/ReturnOp.h>
#include <AUA/Alias/AbstractOps/JoinOp.h>

class AbstractLoop {

private:

    static llvm::BasicBlock* getEntry(const llvm::Loop *l);
    static std::set<llvm::BasicBlock*> getLatches(const llvm::Loop *l);

    const llvm::Loop* loop;

public:

    explicit AbstractLoop(const llvm::Loop *loop);

    llvm::BasicBlock* header;
    llvm::BasicBlock* entry;
    llvm::BasicBlock* exit;
    std::set<llvm::BasicBlock*> latches;

    std::map<llvm::BasicBlock*, AbstractLoop*> getSubAbstractLoopsByHeader();

};

struct LoopLatchInfo {

    JoinOp* headerJoin;
    ReturnOp* headerLastReachable;

    LoopLatchInfo(JoinOp *headerJoin, ReturnOp *headerLastReachable);

};

struct LoopBlockInfo {

    std::map<llvm::BasicBlock*, AbstractLoop*> loopsByHeader;

    std::map<llvm::BasicBlock*, LoopLatchInfo*> loopLatchData;

};

class BBSmallVector : public llvm::SmallVectorImpl<llvm::BasicBlock*> {

public:
    BBSmallVector(unsigned int n);
};


#endif //AUA_ABSTRACTLOOP_H
