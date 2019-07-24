//
// Created by mlaupichler on 15.07.19.
//

#include "AUA/Alias/AbstractOps/ReturnPointerOp.h"


ReturnPointerOp::ReturnPointerOp(PointerFinder *resultFinder, llvm::ReturnInst *returnInst) : ReturnOp(returnInst),
                                                                                              resultFinder(
                                                                                                      resultFinder) {}

void ReturnPointerOp::setResult(Configuration *in) {

    result = resultFinder->findPointers(in);

}
