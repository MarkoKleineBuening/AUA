//
// Created by mlaupichler on 22.06.19.
//

#ifndef AUA_MEMBERPOINTERFINDER_H
#define AUA_MEMBERPOINTERFINDER_H


#include "PointerFinder.h"

class MemberPointerFinder : public PointerFinder {

private:

    const std::string compositeName;
    const int memberIdx;

public:

    MemberPointerFinder(const std::string compositeName, const int memberIdx);

    AbstractPointer * findPointer(Configuration *configuration) const override;

};


#endif //AUA_MEMBERPOINTERFINDER_H
