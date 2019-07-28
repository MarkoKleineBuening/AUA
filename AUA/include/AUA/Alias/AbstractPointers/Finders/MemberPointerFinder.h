//
// Created by mlaupichler on 22.06.19.
//

#ifndef AUA_MEMBERPOINTERFINDER_H
#define AUA_MEMBERPOINTERFINDER_H


#include <llvm/IR/Instructions.h>
#include "PointerFinder.h"
#include "CompositeFinder.h"

class MemberPointerFinder : public PointerFinder {

private:

    const CompositeFinder *compositeFinder;
    const int memberIndex;

    const llvm::GetElementPtrInst* gepInst;

public:

    MemberPointerFinder(const CompositeFinder *compositeFinder, int memberIndex,
                        const PointerFormat &expectedFormat,
                        const llvm::GetElementPtrInst *gepInst);

    PointerSetValue *findPointers(Configuration *configuration) const override;

    std::list<const llvm::Instruction *> getAssociatedInsts() const override;

};


#endif //AUA_MEMBERPOINTERFINDER_H
