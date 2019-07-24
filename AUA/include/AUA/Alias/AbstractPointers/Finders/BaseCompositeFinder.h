//
// Created by mlaupichler on 14.07.19.
//

#ifndef AUA_BASECOMPOSITEFINDER_H
#define AUA_BASECOMPOSITEFINDER_H


#include "CompositeFinder.h"
#include "AUA/Alias/AbstractPointers/CompositeFormat.h"

class BaseCompositeFinder : public CompositeFinder {

private:

    const std::string compositeName;

public:

    BaseCompositeFinder(std::string compositeName,
                        const CompositeFormat &expectedFormat);

    CompositeSetValue *findComposites(Configuration *configuration) const override;

};


#endif //AUA_BASECOMPOSITEFINDER_H
