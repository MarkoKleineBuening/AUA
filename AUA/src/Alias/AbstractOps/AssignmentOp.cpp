//
// Created by mlaupichler on 27.05.19.
//

#include <AUA/Alias/AbstractOps/AssignmentOp.h>
#include <assert.h>

AssignmentOp::AssignmentOp(std::string ptrName, std::string varName) {
    this->pointerName = ptrName;
    this->varName = varName;

}

Configuration* AssignmentOp::apply(Configuration* in) {

    auto pointer = in->pointers[pointerName];
    auto targetVar = in->vars[varName];

    AbstractTarget target = AbstractTarget(targetVar, 0, targetVar->getSize());

    pointer->onlyPointTo(target);

//    in->pointers[pointer->getName()] = pointer;

    return in;

}