#include <utility>

//
// Created by mlaupichler on 27.05.19.
//

#include <AUA/Alias/AbstractOps/AssignmentOp.h>
#include <assert.h>



AssignmentOp::AssignmentOp(const PointerFinder *pointerFinder, const TargetFinder *targetFinder,
                           const llvm::StoreInst *storeInstruction)
        : pointerFinder(pointerFinder), targetFinder(targetFinder), storeInstruction(storeInstruction) {}


Configuration* AssignmentOp::apply(Configuration* in) {

    AbstractPointer* pointer = pointerFinder->findPointer(in);
    AbstractTarget target = targetFinder->findTarget(in);

    assert(target.base->getPointerLevel() == pointer->getPointerLevel() - 1);

    pointer->onlyPointTo(target);

    pointer->setOnlyAssocInst((llvm::Instruction*) storeInstruction);

    return in;

}

std::set<llvm::Instruction *> AssignmentOp::getAssocInstructions() {
    std::set<llvm::Instruction *> result;
    result.insert((llvm::Instruction*) storeInstruction);

    return result;
}
