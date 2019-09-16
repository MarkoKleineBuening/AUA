//
// Created by mlaupichler on 15.09.19.
//

#include "AUA/Alias/AbstractPointers/Finders/NullPointerFinder.h"

PointerSetValue *NullPointerFinder::findPointers(Configuration *configuration) const {
    return new PointerSetValue(expectedFormat);
}

std::list<const llvm::Instruction *> NullPointerFinder::getAssociatedInsts() const {
    return std::list<const llvm::Instruction *>();
}

NullPointerFinder::NullPointerFinder(const PointerFormat &expectedFormat) : PointerFinder(expectedFormat) {

}
