//
// Created by mlaupichler on 27.05.19.
//

#ifndef AUA_POINTERALLOCATIONOP_H
#define AUA_POINTERALLOCATIONOP_H


#include <AUA/Alias/Configuration.h>
#include "PointerOperation.h"

class PointerAllocationOp : public PointerOperation {

private:

    std::string name;

public:

    PointerAllocationOp(std::string name);

    Configuration execute(Configuration in);
};


#endif //AUA_POINTERALLOCATIONOP_H
