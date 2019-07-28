#include <utility>
#include <llvm/IR/Instructions.h>

//
// Created by mlaupichler on 14.07.19.
//

#include "AUA/Alias/AbstractPointers/Finders/BaseCompositeFinder.h"

CompositeSetValue *BaseCompositeFinder::findComposites(Configuration *configuration) const {

    CompositeSetValue *result = new CompositeSetValue(expectedFormat);

    AbstractComposite *comp = configuration->composites[compositeName];
    result->include(comp);

    return result;

}

BaseCompositeFinder::BaseCompositeFinder(const std::string& compositeName, const CompositeFormat &expectedFormat,
                                         const llvm::AllocaInst *allocaInst)
        : CompositeFinder(expectedFormat), compositeName(compositeName), allocaInst(allocaInst) {}

BaseCompositeFinder::BaseCompositeFinder(const std::string& compositeName, const CompositeFormat &expectedFormat)
        : BaseCompositeFinder(compositeName, expectedFormat, nullptr) {}


std::list<const llvm::Instruction *> BaseCompositeFinder::getAssociatedInsts() const {

    auto result = std::list<const llvm::Instruction*>();
    if (allocaInst != nullptr)
        result.push_back(allocaInst);

    return result;

}
