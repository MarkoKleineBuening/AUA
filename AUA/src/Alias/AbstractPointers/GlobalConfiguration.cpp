//
// Created by mlaupichler on 18.07.19.
//

#include <llvm/Support/raw_ostream.h>
#include "AUA/Alias/AbstractPointers/GlobalConfiguration.h"

void GlobalConfiguration::addGlobalFunction(std::string name, AbstractFunction *function) {

    if (GlobalConfiguration::globalFunctions.find(name) != GlobalConfiguration::globalFunctions.end())
        throw GlobalNameAlreadyKnownException();

    GlobalConfiguration::globalFunctions[name] = function;

}

void GlobalConfiguration::addGlobalPointer(std::string name, AbstractPointer *pointer) {

    if (globalPointers.find(name) != globalPointers.end()) throw GlobalNameAlreadyKnownException();

    globalPointers[name] = pointer;

}

void GlobalConfiguration::addGlobalComposite(std::string name, AbstractComposite *composite) {

    if (globalComposites.find(name) != globalComposites.end()) throw GlobalNameAlreadyKnownException();

    globalComposites[name] = composite;

}

void GlobalConfiguration::addGlobalVar(std::string name, AbstractVar *var) {

    if (globalVars.find(name) != globalVars.end()) throw GlobalNameAlreadyKnownException();

    globalVars[name] = var;

}

AbstractFunction *GlobalConfiguration::getGlobalFunction(std::string name) {

    if (globalFunctions.find(name) == globalFunctions.end()) throw UnknownGlobalNameException();

    return globalFunctions[name];

}

AbstractPointer *GlobalConfiguration::getGlobalPointer(std::string name) {

    if (globalPointers.find(name) == globalPointers.end()) throw UnknownGlobalNameException();

    return globalPointers[name];

}

AbstractComposite *GlobalConfiguration::getGlobalComposite(std::string name) {

    if (globalComposites.find(name) == globalComposites.end()) throw UnknownGlobalNameException();

    return globalComposites[name];

}

AbstractVar *GlobalConfiguration::getGlobalVar(std::string name) {

    if (globalVars.find(name) == globalVars.end()) throw UnknownGlobalNameException();

    return globalVars[name];

}
