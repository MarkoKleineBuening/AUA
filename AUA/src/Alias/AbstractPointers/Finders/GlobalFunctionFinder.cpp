#include <utility>

//
// Created by mlaupichler on 21.07.19.
//

#include "AUA/Alias/AbstractPointers/Finders/GlobalFunctionFinder.h"

AbstractFunction *GlobalFunctionFinder::findFunction(Configuration *configuration) const {

    GlobalConfiguration* globalConf = Configuration::global;

    assert(globalConf != nullptr);

    return globalConf->getGlobalFunction(name);

}

GlobalFunctionFinder::GlobalFunctionFinder(const std::string name) : name(name) {}
