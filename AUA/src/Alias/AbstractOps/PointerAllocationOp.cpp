//
// Created by mlaupichler on 27.05.19.
//

#include "AUA/Alias/AbstractOps/PointerAllocationOp.h"

PointerAllocationOp::PointerAllocationOp(std::string name) {
    this->name = name;

}

Configuration* PointerAllocationOp::apply(Configuration* in) {

    AbstractPointer* pointer = new AbstractPointer(0, name);

    in->pointers.insert_or_assign(name, pointer);

    return in;

}