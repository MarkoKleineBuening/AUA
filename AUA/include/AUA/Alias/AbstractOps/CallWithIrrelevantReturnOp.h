//
// Created by mlaupichler on 12.07.19.
//

#ifndef AUA_CALLWITHIRRELEVANTRETURNOP_H
#define AUA_CALLWITHIRRELEVANTRETURNOP_H


#include <llvm/IR/Instructions.h>
#include <AUA/Alias/AbstractPointers/Finders/PointerFinder.h>
#include <AUA/Alias/AbstractPointers/AbstractFunction.h>
#include <AUA/Alias/AbstractPointers/Finders/CompositeFinder.h>
#include <AUA/Alias/AbstractPointers/Finders/FunctionFinder.h>
#include "PointerOperation.h"
#include "SinglePredAndSuccOp.h"

class CallWithIrrelevantReturnOp : public SinglePredAndSuccOp {

private:

    FunctionFinder *functionFinder;
    llvm::CallInst *callInst;


    std::map<int, PointerFinder *> pointerParamFinders;
    std::map<int, CompositeFinder *> compositeParamFinders;

protected:

    Configuration *apply(Configuration *in) override;

public:


    /**
     * Creates new CallOp that executes an alias analysis on the called function. No pointer return is expected for the function.
     * @param functionFinder the function to execute.
     * @param callInst the associated llvm::CallInst object.
     * @param pointerParamFinders PointerFinders for the parameters at the given parameter indices of the function that is executed.
     * @param compositeParamFinders CompositeFinders for the parameters at the given parameter indices of the function that is executed.
     */
    CallWithIrrelevantReturnOp(FunctionFinder *functionFinder, llvm::CallInst *callInst,
                               std::map<int, PointerFinder *> pointerParamFinders,
                               std::map<int, CompositeFinder *> compositeParamFinders);



};

struct CallReturnException : public std::exception {
    const char *what() const throw() {
        return "Function has return value where none was expected.";
    }
};


#endif //AUA_CALLWITHIRRELEVANTRETURNOP_H
