#include <utility>

//
// Created by mlaupichler on 17.07.19.
//

#include "AUA/Alias/AbstractPointers/Finders/DirectCallPointerFinder.h"

PointerSetValue *DirectCallPointerFinder::findPointers(Configuration *configuration) const {

    std::map<int, PointerSetValue *> pointerParams;
    std::map<int, CompositeSetValue *> compositeParams;

    for (auto pp : pointerParamFinders) {

        assert(pp.second != nullptr);
        PointerSetValue* pointers = pp.second->findPointers(configuration);

        assert(pointers != nullptr);

        pointerParams[pp.first] = pointers;
    }

    for (auto cp : compositeParamFinders) {

        compositeParams[cp.first] = cp.second->findComposites(configuration);
    }


    AbstractFunction* function = functionFinder->findFunction(configuration);
    PointerSetValue *returned = function->execute(pointerParams, compositeParams);

    if (returned == nullptr) throw NoReturnValueFromFunctionException();
    if (returned->getFormat() != expectedFormat) throw PointerFinderFormatException(expectedFormat, returned->getFormat());

    return returned;

}

DirectCallPointerFinder::DirectCallPointerFinder(const FunctionFinder *functionFinder,
                                             const std::map<int, PointerFinder *>& pointerParamFinders,
                                             const std::map<int, CompositeFinder *>& compositeParamFinders,
                                             const PointerFormat &expectedFormat, const llvm::CallInst *callInst)
        : CallPointerFinder(expectedFormat, callInst), functionFinder(functionFinder),
          pointerParamFinders(pointerParamFinders), compositeParamFinders(
        compositeParamFinders) {}

