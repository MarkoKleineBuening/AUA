//
// Created by mlaupichler on 18.07.19.
//

#ifndef AUA_FROMPOINTERTARGETFINDER_H
#define AUA_FROMPOINTERTARGETFINDER_H


#include "TargetFinder.h"
#include "PointerFinder.h"

class FromPointerTargetFinder : public TargetFinder {

private:

    const PointerFinder *pointerFinder;

    const llvm::LoadInst* loadInst;


public:

    explicit FromPointerTargetFinder(const PointerFinder *pointerFinder, const llvm::LoadInst *loadInst);

    std::set<AbstractTarget> findTargets(Configuration *conf) const override;

    std::list<const llvm::Instruction *> getAssociatedInsts() const override;

};


#endif //AUA_FROMPOINTERTARGETFINDER_H
