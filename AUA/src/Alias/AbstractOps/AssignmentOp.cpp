#include <utility>

#include <utility>

//
// Created by mlaupichler on 27.05.19.
//

#include <AUA/Alias/AbstractOps/AssignmentOp.h>
#include <assert.h>


AssignmentOp::AssignmentOp(const PointerFinder *pointerFinder, const TargetFinder *targetFinder,
                           const llvm::StoreInst *storeInstruction)
        : pointerFinder(pointerFinder), targetFinder(targetFinder), storeInstruction(storeInstruction) {}


Configuration *AssignmentOp::apply(Configuration *in) {

    auto pointers = pointerFinder->findPointers(in);
    std::set<AbstractTarget> targets = targetFinder->findTargets(in);

    auto insts = targetFinder->getAssociatedInsts();
    insts.push_back(storeInstruction);

    if (pointers->size() == 1) {

        AbstractPointer *pointer = *pointers->asSet().begin();
        pointer->setTargets(targets);

        pointer->setAssocInsts(insts);

    } else {

        for (auto pointer : pointers->asSet()) {

            pointer->addTargets(targets);

            pointer->addAllAssocInsts(insts);

        }
    }

    return in;

}
