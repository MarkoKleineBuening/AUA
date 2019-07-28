//
// Created by mlaupichler on 17.07.19.
//

#ifndef AUA_RETURNEDPOINTERFINDER_H
#define AUA_RETURNEDPOINTERFINDER_H


#include <AUA/Alias/AbstractPointers/AbstractFunction.h>
#include "PointerFinder.h"
#include "CompositeFinder.h"
#include "FunctionFinder.h"

class ReturnedPointerFinder : public PointerFinder {

private:

    const FunctionFinder *functionFinder;
    const std::map<int, PointerFinder *> pointerParamFinders;
    const std::map<int, CompositeFinder *> compositeParamFinders;

    const llvm::CallInst* callInst;

public:

    ReturnedPointerFinder(const FunctionFinder *functionFinder,
                          const std::map<int, PointerFinder *>& pointerParamFinders,
                          const std::map<int, CompositeFinder *>& compositeParamFinders,
                          const PointerFormat &expectedFormat, const llvm::CallInst *callInst);

    PointerSetValue *findPointers(Configuration *configuration) const override;

    std::list<const llvm::Instruction *> getAssociatedInsts() const override;

};

struct NoReturnValueFromFunctionException : public std::exception {
    const char *what() const throw() {
        return "Executed function expecting return pointer but no pointer was returned.";
    }
};


#endif //AUA_RETURNEDPOINTERFINDER_H
