//
// Created by mlaupichler on 18.07.19.
//

#ifndef AUA_FROMPOINTERTARGETFINDER_H
#define AUA_FROMPOINTERTARGETFINDER_H


#include "TargetFinder.h"
#include "PointerFinder.h"

class FromPointerTargetFinder : public TargetFinder {

private:

    PointerFinder *pointerFinder;

public:

    explicit FromPointerTargetFinder(PointerFinder *pointerFinder);

    std::set<AbstractTarget> findTargets(Configuration *conf) const override;

};


#endif //AUA_FROMPOINTERTARGETFINDER_H
