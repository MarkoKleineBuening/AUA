#include <utility>

#include <utility>
#include <llvm/IR/Instructions.h>

//
// Created by mlaupichler on 22.06.19.
//

#include "AUA/Alias/AbstractPointers/Finders/BasePointerFinder.h"

BasePointerFinder::BasePointerFinder(std::string pointerName, const PointerFormat &expectedFormat,
                                     const llvm::AllocaInst *allocaInst)
        : PointerFinder(expectedFormat), pointerName(std::move(pointerName)), allocaInst(allocaInst) {}

BasePointerFinder::BasePointerFinder(std::string pointerName, const PointerFormat &expectedFormat) : BasePointerFinder(std::move(pointerName), expectedFormat, nullptr) {}

PointerSetValue *BasePointerFinder::findPointers(Configuration *configuration) const {

    auto result = new PointerSetValue(expectedFormat);
    AbstractPointer *pointer = configuration->pointers[pointerName];
    result->include(pointer);

    return result;
}

std::list<const llvm::Instruction *> BasePointerFinder::getAssociatedInsts() const {

    auto result = std::list<const llvm::Instruction*>();
    if (allocaInst != nullptr)
        result.push_back(allocaInst);

    return result;

}
