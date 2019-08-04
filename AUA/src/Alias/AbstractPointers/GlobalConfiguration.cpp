//
// Created by mlaupichler on 18.07.19.
//

#include <llvm/Support/raw_ostream.h>
#include "AUA/Alias/AbstractPointers/GlobalConfiguration.h"

GlobalConfiguration::GlobalConfiguration(GlobalValueFactory *globalValueFactory) : globalValueFactory(
        globalValueFactory) {}

AbstractPointer *GlobalConfiguration::getGlobalPointer(const std::string& name) {

    if (globalPointers.find(name) == globalPointers.end()) {

        globalPointers[name] = globalValueFactory->buildGlobalAbstractPointer(name);
    }

    return globalPointers[name];

}

AbstractComposite *GlobalConfiguration::getGlobalComposite(const std::string& name) {

    if (globalComposites.find(name) == globalComposites.end()) {

        globalComposites[name] = globalValueFactory->buildGlobalAbstractComposite(name);
    }

    return globalComposites[name];

}

AbstractVar *GlobalConfiguration::getGlobalVar(const std::string& name) {

    if (globalVars.find(name) == globalVars.end()) {

        globalVars[name] = globalValueFactory->buildGlobalAbstractVar(name);
    }

    return globalVars[name];

}

AbstractReference *GlobalConfiguration::getGlobalReference(const std::string &name) {

    if (globalPointers.find(name) != globalPointers.end()) {
        return globalPointers[name];
    }

    if (globalComposites.find(name) != globalComposites.end()) {
        return globalComposites[name];
    }

    if (globalVars.find(name) != globalVars.end()) {
        return globalVars[name];
    }

    if (globalValueFactory->globalPointerExists(name)) {

        return getGlobalPointer(name);
    }

    if (globalValueFactory->globalCompositeExists(name)) {

        return getGlobalComposite(name);
    }

    if (globalValueFactory->globalVarExists(name)) {

        return getGlobalVar(name);
    }

    throw UnknownGlobalNameException();


}

std::set<AbstractPointer *> GlobalConfiguration::getAllGlobalPointers() {

    std::set<AbstractPointer *> result;
    for (const auto& ptrPair : globalPointers) {

        result.insert(ptrPair.second);
    }

    return result;

}

std::set<AbstractComposite *> GlobalConfiguration::getAllGlobalComposites() {

    std::set<AbstractComposite *> result;
    for (const auto& compPair : globalComposites) {

        result.insert(compPair.second);
    }

    return result;
}

AbstractFunction *GlobalConfiguration::getGlobalFunction(const std::string& name) {

    if (!hasGlobalFunction(name)) {

        // Functions are built when abstracting, so AbstractFunction has to exist at this point.
        throw UnknownGlobalNameException();
    }

    return globalFunctions[name];

}

bool GlobalConfiguration::hasGlobalFunction(const std::string &name) {

    return globalFunctions.find(name) != globalFunctions.end();

}

void GlobalConfiguration::addGlobalFunction(AbstractFunction *function) {

    if (hasGlobalFunction(function->getName())) throw GlobalNameAlreadyKnownException();

    globalFunctions[function->getName()] = function;

}
