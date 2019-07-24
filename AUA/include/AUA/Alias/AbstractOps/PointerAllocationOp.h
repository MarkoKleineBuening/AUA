//
// Created by mlaupichler on 27.05.19.
//

#ifndef AUA_POINTERALLOCATIONOP_H
#define AUA_POINTERALLOCATIONOP_H


#include <AUA/Alias/AbstractPointers/Configuration.h>
#include "SinglePredAndSuccOp.h"
#include <llvm/IR/Instructions.h>


class PointerAllocationOp : public SinglePredAndSuccOp {

private:

    const std::string name;
    const PointerFormat pointerFormat;
    const llvm::AllocaInst *allocInst;


protected:

    Configuration *apply(Configuration *in) override;

    std::set<llvm::Instruction *> getAssocInstructions() override;

public:

    PointerAllocationOp(std::string name, PointerFormat ptrFormat, const llvm::AllocaInst *allocInst);

};


#endif //AUA_POINTERALLOCATIONOP_H
