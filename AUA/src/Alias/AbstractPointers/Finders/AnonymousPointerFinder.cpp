//
// Created by mlaupichler on 25.07.19.
//

#include "AUA/Alias/AbstractPointers/Finders/AnonymousPointerFinder.h"

AnonymousPointerFinder::AnonymousPointerFinder(const PointerFormat &expectedFormat, TargetFinder *targetFinder)
    : PointerFinder(expectedFormat), targetFinder(targetFinder), anonPointerId(anonPointerCounter) {

    anonPointerCounter++;
}

PointerSetValue *AnonymousPointerFinder::findPointers(Configuration *configuration) const {

    auto targets = targetFinder->findTargets(configuration);

    auto anonPointer = new AbstractPointer(buildAnonPointerName(), expectedFormat);
    anonPointer->setTargets(targets);

    auto result = new PointerSetValue(expectedFormat);
    result->include(anonPointer);

    return result;

}

std::string AnonymousPointerFinder::buildAnonPointerName() const {

    std::ostringstream oss;
    oss << "Anonymous Pointer (id: " << anonPointerId << ")";
    return oss.str();

}


