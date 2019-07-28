//
// Created by mlaupichler on 27.05.19.
//

#ifndef AUA_STOREOP_H
#define AUA_STOREOP_H


#include <AUA/Alias/AbstractPointers/Configuration.h>
#include <llvm/IR/Instructions.h>
#include <AUA/Alias/AbstractPointers/Finders/PointerFinder.h>
#include "SinglePredAndSuccOp.h"

class StoreOp : public SinglePredAndSuccOp {

private:
    const PointerFinder *fromFinder;
    const PointerFinder *toFinder;
    const llvm::StoreInst *storeInstruction;
    const std::list<llvm::LoadInst *> loadInstructions;

protected:

    Configuration *apply(Configuration *in) override;


public:

    /**
     * Creates new StoreOp instance.
     * @param fromFinder The PointerFinder for the pointer to take the targets from.
     * @param toFinder The PointerFinder for the pointer to copy the targets to.
     * @param storeInst the associated store instruction.
     */
    StoreOp(PointerFinder *fromFinder, PointerFinder *toFinder, llvm::StoreInst *storeInst);
};


#endif //AUA_STOREOP_H
