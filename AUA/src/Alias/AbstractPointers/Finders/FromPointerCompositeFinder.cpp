#include <utility>

//
// Created by mlaupichler on 18.07.19.
//

#include "AUA/Alias/AbstractPointers/Finders/FromPointerCompositeFinder.h"

FromPointerCompositeFinder::FromPointerCompositeFinder(PointerFinder *pointerFinder, CompositeFormat expectedFormat)
        : CompositeFinder(expectedFormat), pointerFinder(pointerFinder) {}

CompositeSetValue *FromPointerCompositeFinder::findComposites(Configuration *configuration) const {

    PointerSetValue *pointers = pointerFinder->findPointers(configuration);
    CompositeSetValue *result = new CompositeSetValue(expectedFormat);

    for (auto compTarget : pointers->getMergedTargets()) {

        AbstractComposite *comp = dynamic_cast<AbstractComposite *>(compTarget.base);
        result->include(comp);

    }

    return result;


}
