#include <utility>

//
// Created by mlaupichler on 27.05.19.
//

#include <AUA/Alias/AbstractOps/AssignmentOp.h>
#include <assert.h>



AssignmentOp::AssignmentOp(const PointerFinder *pointerFinder, const TargetFinder *targetFinder,
                           const llvm::StoreInst *storeInstruction,
                           const std::list<llvm::GetElementPtrInst *> gepInstructions)
        : pointerFinder(pointerFinder), targetFinder(targetFinder), storeInstruction(storeInstruction),
          gepInstructions(gepInstructions) {}


Configuration* AssignmentOp::apply(Configuration* in) {

    AbstractPointer* pointer = pointerFinder->findPointer(in);
    AbstractTarget target = targetFinder->findTarget(in);

    assert(target.base->getPointerLevel() == pointer->getPointerLevel() - 1);

    pointer->onlyPointTo(target);

    pointer->setAssocInsts(this->getAssocInstructions());

    return in;

}

std::set<llvm::Instruction *> AssignmentOp::getAssocInstructions() {

    std::set<llvm::Instruction *> result;
    result.insert(gepInstructions.begin(), gepInstructions.end());
    result.insert((llvm::Instruction*) storeInstruction);

    return result;
}
