#include <utility>

//
// Created by mlaupichler on 27.05.19.
//

#include <AUA/Alias/AbstractOps/AssignmentOp.h>
#include <assert.h>

AssignmentOp::AssignmentOp(std::string ptrName, std::string varName, llvm::StoreInst *storeInst)
        : pointerName(std::move(ptrName)), targetName(std::move(varName)), storeInstruction(storeInst) {}

Configuration* AssignmentOp::apply(Configuration* in) {

    auto pointer = in->pointers[pointerName];

    int targetLevel = pointer->getLevel() - 1;
    AbstractReference* targetBase;
    int size;

    if (targetLevel == 0) {

        VarRef* targetVar = in->vars[targetName];
        size = targetVar->getSize();
        targetBase = targetVar;

    } else {

        targetBase = in->pointers[targetName];
        size = 0;

    }

    assert(targetBase->getLevel() == pointer->getLevel() - 1);

    AbstractTarget* target = new AbstractTarget(targetBase, 0, size);
    pointer->onlyPointTo(*target);

    pointer->setOnlyAssocInst((llvm::Instruction*) storeInstruction);

    return in;

}

std::set<llvm::Instruction *> AssignmentOp::getAssocInstructions() {
    std::set<llvm::Instruction *> result;
    result.insert((llvm::Instruction*) storeInstruction);

    return result;
}
