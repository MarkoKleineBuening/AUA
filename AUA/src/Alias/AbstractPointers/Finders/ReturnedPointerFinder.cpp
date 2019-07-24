#include <utility>

//
// Created by mlaupichler on 17.07.19.
//

#include "AUA/Alias/AbstractPointers/Finders/ReturnedPointerFinder.h"

PointerSetValue *ReturnedPointerFinder::findPointers(Configuration *configuration) const {

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

ReturnedPointerFinder::ReturnedPointerFinder(FunctionFinder *functionFinder,
                                             std::map<int, PointerFinder *> pointerParamFinders,
                                             std::map<int, CompositeFinder *> compositeParamFinders,
                                             const PointerFormat &expectedFormat) : PointerFinder(
        expectedFormat), functionFinder(functionFinder),
        pointerParamFinders(std::move(pointerParamFinders)), compositeParamFinders(
        std::move(compositeParamFinders)) {}
