//
// Created by mlaupichler on 17.06.19.
//

#include "AUA/Alias/AbstractPointers/Alias.h"


Alias::Alias(const AbstractPointer *ptr1, const AbstractPointer *ptr2, const AbstractTarget &target) : ptr1(ptr1),
                                                                                                       ptr2(ptr2),
                                                                                                       target(target) {}

const std::string Alias::to_String() const {
    std::string result = "";
    result += "(ptr1: " + ptr1->getName() + ", ptr2: " + ptr2->getName() + ", target: " + target.toString() + ")";

    return result;
}

bool Alias::operator==(const Alias &rhs) const {

    if (*ptr1 == *rhs.ptr2 && *ptr2 == *rhs.ptr1)
        return target == rhs.target;

    if (!(*ptr1 == *rhs.ptr1))
        return false;
    if (!(*ptr2 == *rhs.ptr2))
        return false;

    return target == rhs.target;

}
