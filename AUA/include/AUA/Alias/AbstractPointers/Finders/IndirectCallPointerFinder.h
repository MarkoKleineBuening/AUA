//
// Created by mlaupichler on 07.08.19.
//

#ifndef AUA_INDIRECTCALLPOINTERFINDER_H
#define AUA_INDIRECTCALLPOINTERFINDER_H


#include "PointerFinder.h"
#include "CallPointerFinder.h"
#include <llvm/IR/Instructions.h>

/**
 * Handles finding the return pointer for indirect calls. As indirect calls are ignored this delivers a dummy pointer.
 */
class IndirectCallPointerFinder : public CallPointerFinder {

private:

    const int dummyPointerId;

    static int indirectCallCounter;

    const std::string buildPointerName() const;

public:

    IndirectCallPointerFinder(const PointerFormat &expectedFormat, llvm::CallInst *callInst);

    PointerSetValue *findPointers(Configuration *configuration) const override;


};


#endif //AUA_INDIRECTCALLPOINTERFINDER_H
