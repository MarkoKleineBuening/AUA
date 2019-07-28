//
// Created by mlaupichler on 17.07.19.
//

#ifndef AUA_FROMPOINTERPOINTERFINDER_H
#define AUA_FROMPOINTERPOINTERFINDER_H


#include <llvm/IR/Instructions.h>
#include "PointerFinder.h"

class FromPointerPointerFinder : public PointerFinder {

    const PointerFinder *topLevelPointerFinder;
    const int derefDepth;

    const std::list<const llvm::LoadInst*> loadInsts;

public:
    FromPointerPointerFinder(const PointerFinder *topLevelPointerFinder, int derefDepth,
                             const PointerFormat &expectedFormat,
                             const std::list<const llvm::LoadInst *>& loadInsts);

    PointerSetValue *findPointers(Configuration *configuration) const override;

    std::list<const llvm::Instruction *> getAssociatedInsts() const override;

};


#endif //AUA_FROMPOINTERPOINTERFINDER_H
