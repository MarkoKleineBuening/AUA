//
// Created by mlaupichler on 07.08.19.
//

#include "AUA/Alias/AbstractPointers/Finders/IndirectCallPointerFinder.h"

int IndirectCallPointerFinder::indirectCallCounter = 0;

IndirectCallPointerFinder::IndirectCallPointerFinder(const PointerFormat &expectedFormat,
                                       llvm::CallInst *callInst) : CallPointerFinder(expectedFormat, callInst), dummyPointerId(indirectCallCounter++) {}

PointerSetValue *IndirectCallPointerFinder::findPointers(Configuration *configuration) const {

    std::string ptrName = buildPointerName();

    AbstractPointer* dummy = new AbstractPointer(ptrName, expectedFormat);

    auto ptrSet = new PointerSetValue(expectedFormat);
    ptrSet->include(dummy);

    return ptrSet;

}

const std::string IndirectCallPointerFinder::buildPointerName() const {

    std::ostringstream oss;
    oss << "Indirect Call Dummy Pointer (id: " << dummyPointerId << ")";
    return oss.str();


}
