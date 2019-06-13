//
// Created by mlaupichler on 27.05.19.
//

#ifndef AUA_VARALLOCATIONOP_H
#define AUA_VARALLOCATIONOP_H


#include <string>
#include <AUA/Alias/Configuration.h>
#include "SinglePredAndSuccOp.h"

class VarAllocationOp : public SinglePredAndSuccOp {

private:
    std::string name;
    int size;

protected:

    Configuration* apply(Configuration* in) override;

public:

    VarAllocationOp(std::string name, int size);

};


#endif //AUA_VARALLOCATIONOP_H
