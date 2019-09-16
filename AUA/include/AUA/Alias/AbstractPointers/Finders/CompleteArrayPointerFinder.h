//
// Created by mlaupichler on 15.09.19.
//

#ifndef AUA_COMPLETEARRAYPOINTERFINDER_H
#define AUA_COMPLETEARRAYPOINTERFINDER_H


#include <llvm/IR/Instructions.h>
#include "PointerFinder.h"
#include "CompositeFinder.h"

class CompleteArrayPointerFinder : public PointerFinder {

private:

    const CompositeFinder* arrayFinder;

    const llvm::GetElementPtrInst* gepInst;

public:

    CompleteArrayPointerFinder(const CompositeFinder *arrayFinder,
                               const PointerFormat &expectedFormat,
                               const llvm::GetElementPtrInst *gepInst);

    PointerSetValue *findPointers(Configuration *configuration) const override;

    std::list<const llvm::Instruction *> getAssociatedInsts() const override;

};


#endif //AUA_COMPLETEARRAYPOINTERFINDER_H
