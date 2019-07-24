//
// Created by mlaupichler on 15.07.19.
//

#ifndef AUA_RETURNNILOP_H
#define AUA_RETURNNILOP_H


#include <AUA/Alias/AbstractPointers/SetValues/SetValue.h>
#include <llvm/IR/Instructions.h>
#include "ReturnOp.h"

class ReturnNilOp : public ReturnOp {

private:


    void setResult(Configuration *in) override;

public:
    explicit ReturnNilOp(llvm::ReturnInst *returnInst);


};


#endif //AUA_RETURNNILOP_H
