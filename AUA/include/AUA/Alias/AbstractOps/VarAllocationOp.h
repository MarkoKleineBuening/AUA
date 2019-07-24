//
// Created by mlaupichler on 27.05.19.
//

#ifndef AUA_VARALLOCATIONOP_H
#define AUA_VARALLOCATIONOP_H


#include <string>
#include <AUA/Alias/AbstractPointers/Configuration.h>
#include "SinglePredAndSuccOp.h"
#include <llvm/IR/Instructions.h>

class VarAllocationOp : public SinglePredAndSuccOp {

private:
    const std::string name;
    const int size;
    const llvm::AllocaInst *allocaInst;

protected:

    Configuration *apply(Configuration *in) override;

    std::set<llvm::Instruction *> getAssocInstructions() override;

public:

    VarAllocationOp(std::string name, int size, const llvm::AllocaInst *allocaInst);

};


#endif //AUA_VARALLOCATIONOP_H
