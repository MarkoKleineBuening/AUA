//
// Created by mlaupichler on 22.06.19.
//

#ifndef AUA_POINTERFINDER_H
#define AUA_POINTERFINDER_H


#include "AUA/Alias/AbstractPointers/Configuration.h"

class PointerFinder {

public:

    virtual AbstractPointer * findPointer(Configuration *configuration) const = 0;

};


#endif //AUA_POINTERFINDER_H
