//
// Created by mlaupichler on 13.07.19.
//

#ifndef AUA_COMPOSITEFINDER_H
#define AUA_COMPOSITEFINDER_H


#include <AUA/Alias/AbstractPointers/SetValues/CompositeSetValue.h>
#include <AUA/Alias/AbstractPointers/Configuration.h>
#include "AUA/Alias/AbstractPointers/CompositeFormat.h"

class CompositeFinder {

protected:

    const CompositeFormat expectedFormat;

public:

    CompositeFinder(const CompositeFormat &expectedFormat);

    virtual CompositeSetValue *findComposites(Configuration *configuration) const = 0;

};


#endif //AUA_COMPOSITEFINDER_H
