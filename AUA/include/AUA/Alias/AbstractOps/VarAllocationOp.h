//
// Created by mlaupichler on 27.05.19.
//

#ifndef AUA_VARALLOCATIONOP_H
#define AUA_VARALLOCATIONOP_H


#include <string>
#include <AUA/Alias/Configuration.h>
#include "PointerOperation.h"

class VarAllocationOp : public PointerOperation {

private:
    std::string name;
    int size;

public:

    VarAllocationOp(std::string name, int size);

    Configuration execute(Configuration in);

};


#endif //AUA_VARALLOCATIONOP_H
