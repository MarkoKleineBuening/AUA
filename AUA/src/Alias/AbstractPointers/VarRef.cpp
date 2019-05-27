//
// Created by mlaupichler on 16.05.19.
//


#include <AUA/Alias/AbstractPointers/VarRef.h>

VarRef::VarRef(std::string n, int a, int s) {
    name = n;
    alignment = a;
    size = s;
}
