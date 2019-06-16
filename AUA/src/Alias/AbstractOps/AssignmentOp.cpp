//
// Created by mlaupichler on 27.05.19.
//

#include <AUA/Alias/AbstractOps/AssignmentOp.h>
#include <assert.h>

AssignmentOp::AssignmentOp(std::string ptrName, std::string varName, llvm::StoreInst *storeInst)
        : pointerName(ptrName), targetName(varName), storeInstruction(storeInst) {}

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

    return in;

}

std::vector<llvm::Instruction *> AssignmentOp::getAssocInstructions() {
    std::vector<llvm::Instruction *> result;
    result.push_back((llvm::Instruction*) storeInstruction);

    return result;
}
