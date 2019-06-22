//
// Created by mlaupichler on 23.05.19.
//


#include <AUA/Alias/AbstractPointers/AbstractTarget.h>

AbstractTarget::AbstractTarget(AbstractReference * b, int os, int s) : base(b), byteOffset(os), size(s) {}


bool AbstractTarget::operator<(const AbstractTarget &other) const {

    if (*(this->base) < *(other.base))
        return true;
    if (*(other.base) < *(this->base))
        return false;

    return this->byteOffset < other.byteOffset;

}


const std::string AbstractTarget::toString() const {

    std::string typeMsg = (base->getPointerLevel() == 0)? "var" : "ptr lvl " + std::to_string(base->getPointerLevel());
    return "(" + this->base->getName() + ", " +  std::to_string(this->byteOffset) + ", " + std::to_string(this->size) + ", " + typeMsg + ")";
}

bool AbstractTarget::operator>(const AbstractTarget &rhs) const {
    return rhs < *this;
}

bool AbstractTarget::operator<=(const AbstractTarget &rhs) const {
    return !(rhs < *this);
}

bool AbstractTarget::operator>=(const AbstractTarget &rhs) const {
    return !(*this < rhs);
}

bool AbstractTarget::operator==(const AbstractTarget &rhs) const {
    return ((*this <= rhs) && (*this >= rhs));
}
