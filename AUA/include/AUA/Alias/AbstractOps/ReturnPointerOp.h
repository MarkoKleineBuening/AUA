//
// Created by mlaupichler on 15.07.19.
//

#ifndef AUA_RETURNPOINTEROP_H
#define AUA_RETURNPOINTEROP_H


#include <AUA/Alias/AbstractPointers/SetValues/SetValue.h>
#include <AUA/Alias/AbstractPointers/SetValues/PointerSetValue.h>
#include <AUA/Alias/AbstractPointers/Finders/PointerFinder.h>
#include <llvm/IR/Instructions.h>
#include "ReturnOp.h"

class ReturnPointerOp : public ReturnOp {

private:

    PointerFinder *resultFinder;

protected:

    void setResult(Configuration *in) override;

public:

    ReturnPointerOp(PointerFinder *resultFinder, llvm::ReturnInst *returnInst);


};


#endif //AUA_RETURNPOINTEROP_H
