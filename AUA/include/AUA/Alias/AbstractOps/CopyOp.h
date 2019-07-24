//
// Created by mlaupichler on 27.05.19.
//

#ifndef AUA_COPYOP_H
#define AUA_COPYOP_H


#include <AUA/Alias/AbstractPointers/Configuration.h>
#include <llvm/IR/Instructions.h>
#include <AUA/Alias/AbstractPointers/Finders/PointerFinder.h>
#include "SinglePredAndSuccOp.h"

class CopyOp : public SinglePredAndSuccOp {

private:
    const PointerFinder *fromFinder;
    const PointerFinder *toFinder;
    const llvm::StoreInst *storeInstruction;
    const std::list<llvm::LoadInst *> loadInstructions;
    const std::list<llvm::Instruction *> assocInsts;

protected:

    Configuration *apply(Configuration *in) override;

    std::set<llvm::Instruction *> getAssocInstructions() override;

public:

    CopyOp(PointerFinder *fromFinder, PointerFinder *toFinder, llvm::StoreInst *storeInst,
           std::list<llvm::LoadInst *> loadInsts, std::list<llvm::Instruction *> assocInsts);
};


#endif //AUA_COPYOP_H
