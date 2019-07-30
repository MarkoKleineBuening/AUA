#include <utility>

//
// Created by mlaupichler on 28.07.19.
//

#include "AUA/Alias/AbstractPointers/Finders/GlobalCompositeFinder.h"

CompositeSetValue *GlobalCompositeFinder::findComposites(Configuration *configuration) const {

    CompositeSetValue* globalComposite = new CompositeSetValue(expectedFormat);
    globalComposite->include(Configuration::global->getGlobalComposite(name));

    return globalComposite;

}

std::list<const llvm::Instruction *> GlobalCompositeFinder::getAssociatedInsts() const {
    return std::list<const llvm::Instruction *>();
}

GlobalCompositeFinder::GlobalCompositeFinder(std::string name, const CompositeFormat &expectedFormat)
        : CompositeFinder(expectedFormat), name(std::move(name)) {}
