//
// Created by mlaupichler on 23.05.19.
//


#include <AUA/Alias/AbstractPointers/AbstractTarget.h>

AbstractTarget::AbstractTarget(VarRef * b, int os, int s) {

    base = b;
    byteOffset = os;
    size = s;

}

bool AbstractTarget::operator<(const AbstractTarget &other) const {

    if (!(*(this->base) < *(other.base)) && !(*(other.base) < *(this->base))) {

        return this->byteOffset < other.byteOffset;

    }

    return *(this->base) < *(other.base);

}


std::string AbstractTarget::toString() {
 return "(" + this->base->getName() + ", " +  std::to_string(this->byteOffset) + ", " + std::to_string(this->size) + ")";
}