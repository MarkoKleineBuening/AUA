//
// Created by mlaupichler on 07.08.19.
//

#ifndef AUA_CALLPOINTERFINDER_H
#define AUA_CALLPOINTERFINDER_H


#include "PointerFinder.h"
#include <llvm/IR/Instructions.h>

class CallPointerFinder : public PointerFinder {

protected:

    const llvm::CallInst* callInst;

public:

    CallPointerFinder(const PointerFormat &expectedFormat, const llvm::CallInst *callInst);

    std::list<const llvm::Instruction *> getAssociatedInsts() const override;


};


#endif //AUA_CALLPOINTERFINDER_H
