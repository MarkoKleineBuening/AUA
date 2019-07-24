//
// Created by mlaupichler on 13.07.19.
//

#ifndef AUA_MEMBERCOMPOSITEFINDER_H
#define AUA_MEMBERCOMPOSITEFINDER_H


#include "CompositeFinder.h"

class MemberCompositeFinder : public CompositeFinder {

private:

    CompositeFinder *baseCompositeFinder;
    std::list<int> memberIndices;

public:

    MemberCompositeFinder(CompositeFinder *topLevelCompositeFinder,
                          CompositeFormat expectedFormat, std::list<int> memberIndices);


    CompositeSetValue *findComposites(Configuration *configuration) const override;

};


#endif //AUA_MEMBERCOMPOSITEFINDER_H
