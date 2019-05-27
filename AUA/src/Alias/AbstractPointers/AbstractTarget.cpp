//
// Created by mlaupichler on 23.05.19.
//


#include <AUA/Alias/AbstractPointers/AbstractTarget.h>

AbstractTarget::AbstractTarget(VarRef * b, int os, int s) {

    base = b;
    byteOffset = os;
    size = s;

}