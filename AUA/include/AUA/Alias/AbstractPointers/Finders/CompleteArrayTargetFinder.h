//
// Created by mlaupichler on 15.09.19.
//

#ifndef AUA_COMPLETEARRAYTARGETFINDER_H
#define AUA_COMPLETEARRAYTARGETFINDER_H


#include <llvm/IR/Instructions.h>
#include "TargetFinder.h"
#include "CompositeFinder.h"

class CompleteArrayTargetFinder : public TargetFinder {

private:

    const CompositeFinder *arrayFinder;

    const llvm::GetElementPtrInst* gepInst;

public:

    /**
     * Creates a CompleteArrayTargetFinder holding the given information to overapproximate access into an array by a index variable.
     * @param arrayFinder the array of which to return all targets.
     * @param gepInst the associated GetElementPointer Instruction.
     */
    CompleteArrayTargetFinder(const CompositeFinder *arrayFinder, const llvm::GetElementPtrInst *gepInst);

    std::set<AbstractTarget> findTargets(Configuration *conf) const override;

    std::list<const llvm::Instruction *> getAssociatedInsts() const override;
};


#endif //AUA_COMPLETEARRAYTARGETFINDER_H
