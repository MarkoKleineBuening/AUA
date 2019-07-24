//
// Created by mlaupichler on 18.07.19.
//

#ifndef AUA_FROMPOINTERCOMPOSITEFINDER_H
#define AUA_FROMPOINTERCOMPOSITEFINDER_H


#include <AUA/Alias/AbstractPointers/Finders/PointerFinder.h>
#include "CompositeFinder.h"

class FromPointerCompositeFinder : public CompositeFinder {

private:

    PointerFinder *pointerFinder;

public:

    FromPointerCompositeFinder(PointerFinder *pointerFinder, CompositeFormat expectedFormat);

    CompositeSetValue *findComposites(Configuration *configuration) const override;
};


#endif //AUA_FROMPOINTERCOMPOSITEFINDER_H
