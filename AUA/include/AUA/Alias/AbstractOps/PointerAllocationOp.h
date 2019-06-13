//
// Created by mlaupichler on 27.05.19.
//

#ifndef AUA_POINTERALLOCATIONOP_H
#define AUA_POINTERALLOCATIONOP_H


#include <AUA/Alias/Configuration.h>
#include "SinglePredAndSuccOp.h"

class PointerAllocationOp : public SinglePredAndSuccOp {

private:

    std::string name;


protected:

    Configuration* apply(Configuration* in) override;

public:

    PointerAllocationOp(std::string name);

};


#endif //AUA_POINTERALLOCATIONOP_H
