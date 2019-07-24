#include <utility>

//
// Created by mlaupichler on 21.07.19.
//

#include "AUA/Alias/AbstractPointers/Finders/GlobalFunctionFinder.h"

AbstractFunction *GlobalFunctionFinder::findFunction(Configuration *configuration) {

    GlobalConfiguration* globalConf = Configuration::global;

    assert(globalConf != nullptr);

    return globalConf->getGlobalFunction(name);

}

GlobalFunctionFinder::GlobalFunctionFinder(std::string name) : name(std::move(name)) {}
