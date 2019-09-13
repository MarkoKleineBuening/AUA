//
// Created by mlaupichler on 06.08.19.
//

#include "AUA/Alias/AbstractPointers/AbstractLoop.h"
#include <llvm/ADT/SmallVector.h>



AbstractLoop::AbstractLoop(const llvm::Loop *loop) : loop(loop), header(loop->getHeader()), exit(loop->getExitBlock()),
                                                     entry(getEntry(loop)), latches(getLatches(loop)) {}

llvm::BasicBlock *AbstractLoop::getEntry(const llvm::Loop *l) {

    auto term = l->getHeader()->getTerminator();
    assert(term->getNumSuccessors() == 2);

    if (!l->isLoopExiting(term->getSuccessor(0))) {
        assert(term->getSuccessor(0) != nullptr);
        return term->getSuccessor(0);
    }

    assert(!l->isLoopExiting(term->getSuccessor(1)));
    assert(term->getSuccessor(1) != nullptr);
    return term->getSuccessor(1);

    BBSmallVector exitBlocks = BBSmallVector(0);
    l->getExitBlocks(exitBlocks);

    llvm::outs() << "Exit blocks: ";
    for (auto bb : exitBlocks) {
        llvm::outs() << bb->getName() << " ";
    }
    llvm::outs() << "\n";


    llvm::outs() << "Second successor: " << term->getSuccessor(1)->getName() << "\n";
    llvm::outs() << "Exit block: " << l->getExitBlock() << "\n";


}

std::set<llvm::BasicBlock *> AbstractLoop::getLatches(const llvm::Loop *l) {

//    const int latchCount = std::distance(llvm::pred_begin(l->getHeader()), llvm::pred_end(l->getHeader())) - 1;

    llvm::SmallVector<llvm::BasicBlock*, 1> latches;
    l->getLoopLatches(latches);

    std::set<llvm::BasicBlock*> latchSet;
    for(auto latch : latches) {
        latchSet.insert(latch);
    }

    return latchSet;

}

std::map<llvm::BasicBlock *, AbstractLoop *> AbstractLoop::getSubAbstractLoopsByHeader() {

    std::map<llvm::BasicBlock *, AbstractLoop *> subAbstractLoopsByHeader;

    for (auto subLoop : loop->getSubLoops()) {

        auto abstractSub = new AbstractLoop(subLoop);
        subAbstractLoopsByHeader.emplace(abstractSub->header, abstractSub);
    }

    return subAbstractLoopsByHeader;
}

LoopLatchInfo::LoopLatchInfo(JoinOp *headerJoin, ReturnOp *headerLastReachable) : headerJoin(headerJoin),
                                                                                  headerLastReachable(
                                                                                          headerLastReachable) {}

BBSmallVector::BBSmallVector(unsigned int n) : SmallVectorImpl(n) {}
