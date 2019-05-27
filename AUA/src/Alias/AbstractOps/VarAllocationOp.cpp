//
// Created by mlaupichler on 27.05.19.
//

#include "AUA/Alias/AbstractOps/VarAllocationOp.h"

VarAllocationOp::VarAllocationOp(std::string name, int size) {

    this->name = name;
    this->size = size;
}

Configuration VarAllocationOp::execute(Configuration in) {

    VarRef* var = new VarRef(name, 0, size);

    in.vars.insert_or_assign(name, var);

    return in;
}