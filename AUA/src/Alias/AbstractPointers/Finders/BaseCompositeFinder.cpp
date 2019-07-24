#include <utility>

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

BaseCompositeFinder::BaseCompositeFinder(std::string compositeName, const CompositeFormat &expectedFormat)
        : CompositeFinder(expectedFormat), compositeName(std::move(compositeName)) {}
