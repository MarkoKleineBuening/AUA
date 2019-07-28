#include <utility>

//
// Created by mlaupichler on 27.05.19.
//

#include "AUA/Alias/AbstractOps/StoreOp.h"
#include <llvm/Support/FileSystem.h>
#include <llvm/IR/Instructions.h>

StoreOp::StoreOp(PointerFinder *fromFinder, PointerFinder *toFinder, llvm::StoreInst *storeInst)
        : fromFinder(fromFinder), toFinder(toFinder), storeInstruction(storeInst) {}

Configuration *StoreOp::apply(Configuration *in) {

    assert(fromFinder != nullptr);
    assert(toFinder != nullptr);

    auto toPointers = toFinder->findPointers(in);
    auto fromPointers = fromFinder->findPointers(in);


    assert(fromPointers->getFormat() == toPointers->getFormat());
    auto insts = fromPointers->getMergedAssociatedInsts();
    auto fromAssociatedInsts = fromFinder->getAssociatedInsts();
    insts.insert(insts.end(), fromAssociatedInsts.begin(), fromAssociatedInsts.end());
    insts.push_back(storeInstruction);

    std::set<AbstractTarget> allTargetsOfFrom = fromPointers->getMergedTargets();

    if (toPointers->size() == 1) {

        AbstractPointer *toPointer = *(toPointers->asSet().begin());
        toPointer->setTargets(allTargetsOfFrom);
        toPointer->setAssocInsts(insts);

    } else {

        for (auto toPointer : toPointers->asSet()) {

            toPointer->addTargets(allTargetsOfFrom);
            toPointer->addAllAssocInsts(insts);
        }
    }

    return in;

}

