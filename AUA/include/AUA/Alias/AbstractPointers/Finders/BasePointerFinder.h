//
// Created by mlaupichler on 22.06.19.
//

#ifndef AUA_BASEPOINTERFINDER_H
#define AUA_BASEPOINTERFINDER_H


#include <AUA/Alias/AbstractPointers/SetValues/PointerSetValue.h>
#include "PointerFinder.h"

class BasePointerFinder : public PointerFinder {

private:

    const std::string pointerName;

public:

    BasePointerFinder(std::string pointerName, const PointerFormat &expectedFormat);

    PointerSetValue *findPointers(Configuration *configuration) const override;

};


#endif //AUA_BASEPOINTERFINDER_H
