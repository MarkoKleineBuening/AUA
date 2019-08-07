//
// Created by mlaupichler on 17.07.19.
//

#ifndef AUA_DIRECTCALLPOINTERFINDER_H
#define AUA_DIRECTCALLPOINTERFINDER_H


#include <AUA/Alias/AbstractPointers/AbstractFunction.h>
#include "PointerFinder.h"
#include "CompositeFinder.h"
#include "FunctionFinder.h"
#include "CallPointerFinder.h"

class DirectCallPointerFinder : public CallPointerFinder {

private:

    const FunctionFinder *functionFinder;
    const std::map<int, PointerFinder *> pointerParamFinders;
    const std::map<int, CompositeFinder *> compositeParamFinders;


public:

    DirectCallPointerFinder(const FunctionFinder *functionFinder,
                          const std::map<int, PointerFinder *>& pointerParamFinders,
                          const std::map<int, CompositeFinder *>& compositeParamFinders,
                          const PointerFormat &expectedFormat, const llvm::CallInst *callInst);

    PointerSetValue *findPointers(Configuration *configuration) const override;


};

struct NoReturnValueFromFunctionException : public std::exception {
    const char *what() const throw() {
        return "Executed function expecting return pointer but no pointer was returned.";
    }
};


#endif //AUA_DIRECTCALLPOINTERFINDER_H
