//
// Created by mlaupichler on 15.09.19.
//

#include "AUA/Alias/AbstractPointers/Finders/NullCompositeFinder.h"

NullCompositeFinder::NullCompositeFinder(const CompositeFormat &expectedFormat) : CompositeFinder(expectedFormat) {

}

CompositeSetValue *NullCompositeFinder::findComposites(Configuration *configuration) const {
    return new CompositeSetValue(expectedFormat);
}

std::list<const llvm::Instruction *> NullCompositeFinder::getAssociatedInsts() const {
    return std::list<const llvm::Instruction *>();
}
