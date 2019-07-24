//
// Created by mlaupichler on 17.06.19.
//

#ifndef AUA_ALIAS_H
#define AUA_ALIAS_H


#include <ostream>
#include "AbstractTarget.h"
#include "AbstractPointer.h"

struct Alias {

    Alias(const AbstractPointer *ptr1, const AbstractPointer *ptr2, const AbstractTarget &target);

    bool operator==(const Alias &rhs) const;

    const std::string to_String() const;

    const AbstractPointer *ptr1;
    const AbstractPointer *ptr2;
    const AbstractTarget target;

};


#endif //AUA_ALIAS_H
