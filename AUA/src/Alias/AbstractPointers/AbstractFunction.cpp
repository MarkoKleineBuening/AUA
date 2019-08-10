#include <utility>
#include <AUA/Alias/AbstractPointers/ConfigurationPrinter.h>

//
// Created by mlaupichler on 10.07.19.
//

#include "AUA/Alias/AbstractPointers/AbstractFunction.h"


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


PointerSetValue *AbstractFunction::execute(std::map<int, PointerSetValue *> ptrParams,
                                           std::map<int, CompositeSetValue *> compParams) {

    llvm::outs() << "\n\n------------------------------ \n\n";
    llvm::outs() << "Function " << name << " called.\n";


    for (auto ptrFormatPair : *ptrParamFormats) {
        assert(ptrParams.find(ptrFormatPair.first) != ptrParams.end());
    }

    for (const auto& compFormatPair : *compParamFormats) {
        assert(compParams.find(compFormatPair.first) != compParams.end());
    }


    auto conf = new Configuration();

    ReferenceFlags paramFlags = ReferenceFlags(false, false, true);

    for (auto var : *varParams) {

        conf->vars[var->getName()] = var;
    }

    for (auto PI = ptrParams.begin(), PE = ptrParams.end(); PI != PE; ++PI) {

        auto ptrParamPair = *PI;
        int paramIndex = ptrParamPair.first;
        PointerSetValue *pointers = ptrParamPair.second;

        assert(ptrParamFormats->find(paramIndex) != ptrParamFormats->end());
        assert(ptrParamFormats->at(paramIndex) == pointers->getFormat());

        AbstractPointer *param = pointers->mergeToNewPointer(paramNames[paramIndex], paramFlags);

        conf->pointers[param->getName()] = param;

    }

    for (auto CI = compParams.begin(), CE = compParams.end(); CI != CE; ++CI) {

        auto compParamPair = *CI;
        int paramIndex = compParamPair.first;
        CompositeSetValue *composites = compParamPair.second;

        assert(compParamFormats->find(paramIndex) != compParamFormats->end());
        assert(compParamFormats->at(paramIndex) == composites->getFormat());

        AbstractComposite *param = composites->mergeToNewComposite(paramNames[paramIndex], paramFlags);

        conf->composites[param->getName()] = param;

    }


    initialOp->execute(conf);

    lastConfiguration = finalOp->getLastConfiguration();

    llvm::outs() << "\nLast configuration of " << name << " is: \n\n";
    ConfigurationPrinter::printFullInfoVerbose(lastConfiguration);

    llvm::outs() << "\n\n------------------------------ \n\n";

    auto functionReturn = finalOp->getResult();


    return functionReturn;

}

void AbstractFunction::executeAsEntry() {

    auto emptyInputPointers = getEmptyInputPointers();
    auto emptyInputComposites = getEmptyInputComposites();

    execute(emptyInputPointers, emptyInputComposites);

}

std::map<int, PointerSetValue*> AbstractFunction::getEmptyInputPointers() {

    std::map<int, PointerSetValue*> emptyPointers;
    for (auto pfp: *ptrParamFormats) {

        std::string ptrName = "emptyInputPtr";
        ptrName += std::to_string(pfp.first);

        auto emptyPointer = new AbstractPointer(ptrName, pfp.second);
        auto emptyPointerSV = new PointerSetValue(pfp.second);
        emptyPointerSV->include(emptyPointer);

        emptyPointers[pfp.first] = emptyPointerSV;
    }

    return emptyPointers;

}

std::map<int, CompositeSetValue*> AbstractFunction::getEmptyInputComposites() {

    std::map<int, CompositeSetValue*> emptyComposites;
    for (const auto& cfp: *compParamFormats) {

        std::string compName = "emptyInputComp";
        compName += std::to_string(cfp.first);

        auto emptyComposite = new AbstractComposite(compName, cfp.second);
        auto emptyCompositeSV = new CompositeSetValue(cfp.second);
        emptyCompositeSV->include(emptyComposite);

        emptyComposites[cfp.first] = emptyCompositeSV;
    }

    return emptyComposites;

}




