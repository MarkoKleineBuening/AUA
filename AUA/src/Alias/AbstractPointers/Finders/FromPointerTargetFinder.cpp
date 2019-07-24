//
// Created by mlaupichler on 18.07.19.
//

#include "AUA/Alias/AbstractPointers/Finders/FromPointerTargetFinder.h"

std::set<AbstractTarget> FromPointerTargetFinder::findTargets(Configuration *conf) const {
    return pointerFinder->findPointers(conf)->getMergedTargets();
}

FromPointerTargetFinder::FromPointerTargetFinder(PointerFinder *pointerFinder) : pointerFinder(pointerFinder) {}
