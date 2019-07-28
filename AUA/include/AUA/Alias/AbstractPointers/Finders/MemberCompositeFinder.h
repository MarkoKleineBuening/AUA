//
// Created by mlaupichler on 13.07.19.
//

#ifndef AUA_MEMBERCOMPOSITEFINDER_H
#define AUA_MEMBERCOMPOSITEFINDER_H


#include <llvm/IR/Instructions.h>
#include "CompositeFinder.h"

class MemberCompositeFinder : public CompositeFinder {

private:

    const CompositeFinder *topLevelCompositeFinder;
    const std::list<int> memberIndices;

    const std::list<llvm::GetElementPtrInst*> gepInsts;

public:

    MemberCompositeFinder(const CompositeFinder *topLevelCompositeFinder,
                          const CompositeFormat &expectedFormat, const std::list<int>& memberIndices,
                          const std::list<llvm::GetElementPtrInst *>& gepInsts);


    CompositeSetValue *findComposites(Configuration *configuration) const override;

    std::list<const llvm::Instruction *> getAssociatedInsts() const override;

};


#endif //AUA_MEMBERCOMPOSITEFINDER_H
