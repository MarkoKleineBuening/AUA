//
// Created by mlaupichler on 22.06.19.
//

#ifndef AUA_MEMBERPOINTERFINDER_H
#define AUA_MEMBERPOINTERFINDER_H


#include "PointerFinder.h"
#include "CompositeFinder.h"

class MemberPointerFinder : public PointerFinder {

private:

    const CompositeFinder *compositeFinder;
    const int memberIndex;

public:

    MemberPointerFinder(const CompositeFinder *compositeFinder, const int memberIndex,
                        const PointerFormat &expectedFormat);

    PointerSetValue *findPointers(Configuration *configuration) const override;

};


#endif //AUA_MEMBERPOINTERFINDER_H
