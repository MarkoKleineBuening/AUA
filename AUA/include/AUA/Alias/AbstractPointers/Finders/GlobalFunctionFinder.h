//
// Created by mlaupichler on 21.07.19.
//

#ifndef AUA_GLOBALFUNCTIONFINDER_H
#define AUA_GLOBALFUNCTIONFINDER_H


#include "FunctionFinder.h"
#include <AUA/Alias/AbstractPointers/GlobalConfiguration.h>

class GlobalFunctionFinder : public FunctionFinder {

private:

    std::string name;

public:

    explicit GlobalFunctionFinder(std::string name);

    AbstractFunction *findFunction(Configuration *configuration) override;

};


#endif //AUA_GLOBALFUNCTIONFINDER_H
