//
// Created by mlaupichler on 23.05.19.
//

#ifndef AUA_ABSTRACTTARGET_H
#define AUA_ABSTRACTTARGET_H


#include "AbstractVar.h"

struct AbstractTarget {

public:

    AbstractTarget(AbstractReference *base, int offset, int size);

    bool operator<(const AbstractTarget &other) const;

    bool operator>(const AbstractTarget &rhs) const;

    bool operator<=(const AbstractTarget &rhs) const;

    bool operator>=(const AbstractTarget &rhs) const;

    bool operator==(const AbstractTarget &rhs) const;

    const std::string toString() const;

    AbstractReference *base;
    const int byteOffset;
    const int size;

};


#endif //AUA_ABSTRACTTARGET_H
