//
// Created by mlaupichler on 12.05.19.
//

#include "Reference.h"



Reference::Reference(uint64_t s, RefType t, std::string n) {
    id = 0;
    name = n;
    size = s;
    type = t;
}
