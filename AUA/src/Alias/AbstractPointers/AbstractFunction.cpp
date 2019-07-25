#include <utility>

//
// Created by mlaupichler on 10.07.19.
//

#include "AUA/Alias/AbstractPointers/AbstractFunction.h"

PointerSetValue *AbstractFunction::execute(std::map<int, PointerSetValue *> ptrParams,
                                           std::map<int, CompositeSetValue *> compParams) {

    llvm::outs() << "\n\n------------------------------ \n\n";
    llvm::outs() << "Function " << name << " called.\n";

    Configuration *conf = new Configuration();

    for (auto var : *varParams) {
        conf->vars[var->getName()] = var;
    }

    for (auto PI = ptrParams.begin(), PE = ptrParams.end(); PI != PE; ++PI) {

        auto ptrParamPair = *PI;
        int paramIndex = ptrParamPair.first;
        PointerSetValue *pointers = ptrParamPair.second;

        assert(ptrParamFormats->find(paramIndex) != ptrParamFormats->end());
        assert(ptrParamFormats->at(paramIndex) == pointers->getFormat());

        AbstractPointer *param = pointers->mergeToNewPointer(paramNames[paramIndex]);

        //TODO associated instructions
        conf->pointers[param->getName()] = param;

    }

    for (auto CI = compParams.begin(), CE = compParams.end(); CI != CE; ++CI) {

        auto compParamPair = *CI;
        int paramIndex = compParamPair.first;
        CompositeSetValue *composites = compParamPair.second;

        assert(compParamFormats->find(paramIndex) != compParamFormats->end());
        assert(compParamFormats->at(paramIndex) == composites->getFormat());

        AbstractComposite *param = composites->mergeToNewComposite(paramNames[paramIndex]);

        conf->composites[param->getName()] = param;

    }


    initialOp->execute(conf);

    llvm::outs() << "\n\n------------------------------ \n\n";

    lastConfiguration = finalOp->getLastConfiguration();
    auto functionReturn = finalOp->getResult();

    return functionReturn;

}

AbstractFunction::AbstractFunction(DummyInitialOp *initialOp, ReturnOp *finalOp, std::string name,
                                   std::map<int, PointerFormat> *ptrParamFormats,
                                   std::map<int, CompositeFormat> *compParamFormats,
                                   const std::set<AbstractVar*>* varParams,
                                   std::vector<std::string> paramNames)
        : initialOp(initialOp), finalOp(finalOp), name(std::move(name)), ptrParamFormats(ptrParamFormats),
          compParamFormats(compParamFormats), paramNames(std::move(paramNames)), varParams(varParams) {

    assert(initialOp != nullptr);
    assert(finalOp != nullptr);

}

const std::string &AbstractFunction::getName() const {
    return name;
}

Configuration *AbstractFunction::getLastConfiguration() const {
    return lastConfiguration;
}


