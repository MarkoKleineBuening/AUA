//
// Created by mlaupichler on 15.07.19.
//

#include "AUA/Alias/AbstractOps/ReturnNilOp.h"


void ReturnNilOp::setResult(Configuration *in) {

    result = nullptr;

}

ReturnNilOp::ReturnNilOp(llvm::ReturnInst *returnInst) : ReturnOp(returnInst) {

}
