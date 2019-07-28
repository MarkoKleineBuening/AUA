//
// Created by mlaupichler on 25.07.19.
//

#ifndef AUA_ANONYMOUSPOINTERFINDER_H
#define AUA_ANONYMOUSPOINTERFINDER_H


#include "PointerFinder.h"
#include "TargetFinder.h"

class AnonymousPointerFinder : public PointerFinder {

private:

    const int anonPointerId;

    TargetFinder* targetFinder;
    std::string buildAnonPointerName() const;

public:

    static int anonPointerCounter;

    AnonymousPointerFinder(const PointerFormat &expectedFormat, TargetFinder *targetFinder);

    PointerSetValue *findPointers(Configuration *configuration) const override;

    std::list<const llvm::Instruction *> getAssociatedInsts() const override;

};





struct TooManyAnonymousPointersException : public std::exception {
    const char *what() const throw() {
        return "An unknown instruction was encountered while trying to build a Finder.";
    }
};


#endif //AUA_ANONYMOUSPOINTERFINDER_H
