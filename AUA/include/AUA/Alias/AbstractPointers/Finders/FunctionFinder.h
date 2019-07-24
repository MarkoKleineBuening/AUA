//
// Created by mlaupichler on 21.07.19.
//

#ifndef AUA_FUNCTIONFINDER_H
#define AUA_FUNCTIONFINDER_H


#include <AUA/Alias/AbstractPointers/AbstractFunction.h>

class FunctionFinder {

public:

    virtual AbstractFunction *findFunction(Configuration *configuration) = 0;

};


#endif //AUA_FUNCTIONFINDER_H
