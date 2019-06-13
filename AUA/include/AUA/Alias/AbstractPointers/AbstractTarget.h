//
// Created by mlaupichler on 23.05.19.
//

#ifndef AUA_ABSTRACTTARGET_H
#define AUA_ABSTRACTTARGET_H


#include "VarRef.h"

struct AbstractTarget {

public:

    AbstractTarget(VarRef* base, int offset, int size);

    bool operator <(const AbstractTarget & other) const;

    std::string toString();

    VarRef* base;
    int byteOffset;
    int size;

};


#endif //AUA_ABSTRACTTARGET_H
