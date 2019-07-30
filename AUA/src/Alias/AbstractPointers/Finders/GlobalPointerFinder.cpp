#include <utility>

//
// Created by mlaupichler on 28.07.19.
//

#include "AUA/Alias/AbstractPointers/Finders/GlobalPointerFinder.h"

PointerSetValue *GlobalPointerFinder::findPointers(Configuration *configuration) const {

    PointerSetValue* globalPointer = new PointerSetValue(expectedFormat);
    globalPointer->include(Configuration::global->getGlobalPointer(name));

    return globalPointer;

}

std::list<const llvm::Instruction *> GlobalPointerFinder::getAssociatedInsts() const {
    return std::list<const llvm::Instruction *>();
}

GlobalPointerFinder::GlobalPointerFinder(std::string name, const PointerFormat &expectedFormat) : PointerFinder(
        expectedFormat), name(std::move(name)) {}
