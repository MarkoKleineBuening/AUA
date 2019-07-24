//
// Created by mlaupichler on 21.06.19.
//

#ifndef AUA_COMPOSITEALLOCATIONOP_H
#define AUA_COMPOSITEALLOCATIONOP_H


#include "SinglePredAndSuccOp.h"
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/DataLayout.h>
#include <llvm/IR/Instructions.h>

class CompositeAllocationOp : public SinglePredAndSuccOp {

    llvm::CompositeType *compType;
    llvm::DataLayout *dl;
    std::string name;
    llvm::AllocaInst *allocaInst;

protected:

    std::set<llvm::Instruction *> getAssocInstructions() override;

    Configuration *apply(Configuration *in) override;

public:

    CompositeAllocationOp(llvm::CompositeType *compType, llvm::DataLayout *dl,
                          std::string name, llvm::AllocaInst *allocaInst);

};


#endif //AUA_COMPOSITEALLOCATIONOP_H
