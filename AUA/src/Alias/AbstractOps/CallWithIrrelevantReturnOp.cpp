#include <utility>


//
// Created by mlaupichler on 12.07.19.
//

#include "AUA/Alias/AbstractOps/CallWithIrrelevantReturnOp.h"


Configuration *CallWithIrrelevantReturnOp::apply(Configuration *in) {

    std::map<int, PointerSetValue *> pointerParams;
    std::map<int, CompositeSetValue *> compositeParams;

    for (auto ptrFinderPair : pointerParamFinders) {

        pointerParams[ptrFinderPair.first] = ptrFinderPair.second->findPointers(in);
    }

    for (auto compFinderPair : compositeParamFinders) {

        compositeParams[compFinderPair.first] = compFinderPair.second->findComposites(in);
    }

    AbstractFunction *function = functionFinder->findFunction(in);

    PointerSetValue *callResult = function->execute(pointerParams, compositeParams);



    if (callResult != nullptr) throw CallReturnException();

    return in;

}


CallWithIrrelevantReturnOp::CallWithIrrelevantReturnOp(FunctionFinder *functionFinder, llvm::CallInst *callInst,
                                                       std::map<int, PointerFinder *> pointerParamFinders,
                                                       std::map<int, CompositeFinder *> compositeParamFinders)
        : functionFinder(functionFinder), callInst(callInst), pointerParamFinders(std::move(pointerParamFinders)),
          compositeParamFinders(std::move(compositeParamFinders)) {}
