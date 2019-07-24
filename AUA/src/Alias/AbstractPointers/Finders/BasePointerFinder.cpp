#include <utility>

//
// Created by mlaupichler on 22.06.19.
//

#include "AUA/Alias/AbstractPointers/Finders/BasePointerFinder.h"

BasePointerFinder::BasePointerFinder(std::string pointerName, const PointerFormat &expectedFormat)
        : PointerFinder(expectedFormat), pointerName(std::move(pointerName)) {}

PointerSetValue *BasePointerFinder::findPointers(Configuration *configuration) const {

    auto result = new PointerSetValue(expectedFormat);
    AbstractPointer *pointer = configuration->pointers[pointerName];
    result->include(pointer);

    return result;
}
