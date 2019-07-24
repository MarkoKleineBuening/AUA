#include <utility>

#include <utility>

//
// Created by mlaupichler on 27.05.19.
//

#include <AUA/Alias/AbstractOps/AssignmentOp.h>
#include <assert.h>


AssignmentOp::AssignmentOp(const PointerFinder *pointerFinder, const TargetFinder *targetFinder,
                           const llvm::StoreInst *storeInstruction,
                           std::list<llvm::Instruction *> assocInsts)
        : pointerFinder(pointerFinder), targetFinder(targetFinder), storeInstruction(storeInstruction),
          assocInsts(std::move(assocInsts)) {}


Configuration *AssignmentOp::apply(Configuration *in) {

    auto pointers = pointerFinder->findPointers(in);
    std::set<AbstractTarget> targets = targetFinder->findTargets(in);

    if (pointers->size() == 1) {

        AbstractPointer *pointer = *pointers->asSet().begin();
        pointer->setTargets(targets);

    } else {

        for (auto pointer : pointers->asSet()) {

            pointer->addTargets(targets);
        }
    }

    return in;

}

std::set<llvm::Instruction *> AssignmentOp::getAssocInstructions() {

    std::set<llvm::Instruction *> result;
    result.insert(assocInsts.begin(), assocInsts.end());
    result.insert((llvm::Instruction *) storeInstruction);

    return result;
}
