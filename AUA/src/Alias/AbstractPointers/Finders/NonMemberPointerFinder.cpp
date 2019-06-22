//
// Created by mlaupichler on 22.06.19.
//

#include "AUA/Alias/AbstractPointers/Finders/NonMemberPointerFinder.h"

NonMemberPointerFinder::NonMemberPointerFinder(const std::string &pointerName) : pointerName(pointerName) {}

AbstractPointer * NonMemberPointerFinder::findPointer(Configuration *configuration) const {
    return configuration->pointers[pointerName];
}
