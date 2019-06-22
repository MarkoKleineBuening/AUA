//
// Created by mlaupichler on 22.06.19.
//

#ifndef AUA_NONMEMBERPOINTERFINDER_H
#define AUA_NONMEMBERPOINTERFINDER_H


#include "PointerFinder.h"

class NonMemberPointerFinder : public PointerFinder {

private:

    const std::string pointerName;

public:

    NonMemberPointerFinder(const std::string &pointerName);

    AbstractPointer * findPointer(Configuration *configuration) const override;

};


#endif //AUA_NONMEMBERPOINTERFINDER_H
