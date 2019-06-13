//
// Created by mlaupichler on 27.05.19.
//

#include "AUA/Alias/AbstractOps/CopyOp.h"

CopyOp::CopyOp(std::string fromName, std::string toName) {
    this->fromName = fromName;
    this->toName = toName;
}

Configuration* CopyOp::apply(Configuration* in) {

    auto from = in->pointers[fromName];
    auto to = in->pointers[toName];

    to->copyTargetsFrom(from);

    in->pointers[to->getName()] = to;

    return in;

}