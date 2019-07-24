//
// Created by mlaupichler on 17.07.19.
//

#ifndef AUA_FROMPOINTERPOINTERFINDER_H
#define AUA_FROMPOINTERPOINTERFINDER_H


#include "PointerFinder.h"

class FromPointerPointerFinder : public PointerFinder {

    PointerFinder *topLevelPointerFinder;
    int derefDepth;

public:
    FromPointerPointerFinder(PointerFinder *topLevelPointerFinder, int derefDepth,
                             const PointerFormat &expectedFormat);

    PointerSetValue *findPointers(Configuration *configuration) const override;

};


#endif //AUA_FROMPOINTERPOINTERFINDER_H
