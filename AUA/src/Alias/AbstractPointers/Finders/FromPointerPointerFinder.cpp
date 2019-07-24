//
// Created by mlaupichler on 17.07.19.
//

#include "AUA/Alias/AbstractPointers/Finders/FromPointerPointerFinder.h"

PointerSetValue *FromPointerPointerFinder::findPointers(Configuration *configuration) const {

    PointerSetValue *topLevelPointers = topLevelPointerFinder->findPointers(configuration);

    auto pointerTargets = topLevelPointers->getMergedTargets(derefDepth);

    auto result = new PointerSetValue(PointerFormat(expectedFormat));

    for (AbstractTarget target : pointerTargets) {

        auto resultPointer = dynamic_cast<AbstractPointer *>(target.base);

        assert(resultPointer != nullptr);

        result->include(resultPointer);
    }

    return result;

}

FromPointerPointerFinder::FromPointerPointerFinder(PointerFinder *topLevelPointerFinder, int derefDepth,
                                                   const PointerFormat &expectedFormat)
        : PointerFinder(expectedFormat), derefDepth(derefDepth), topLevelPointerFinder(topLevelPointerFinder) {}
