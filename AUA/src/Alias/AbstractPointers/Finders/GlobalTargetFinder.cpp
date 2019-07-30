//
// Created by mlaupichler on 28.07.19.
//

#include <set>
#include <AUA/Alias/AbstractPointers/Configuration.h>
#include "AUA/Alias/AbstractPointers/Finders/GlobalTargetFinder.h"

std::set <AbstractTarget> GlobalTargetFinder::findTargets(Configuration *conf) const {

    AbstractReference *targetBase;
    int size;

    if (Configuration::global->hasGlobalVar(name)) {

        AbstractVar *targetVar = Configuration::global->getGlobalVar(name);
        size = targetVar->getSize();
        targetBase = targetVar;

    } else if (Configuration::global->hasGlobalPointer(name)) {

        targetBase = Configuration::global->getGlobalPointer(name);
        size = 0;

    } else if (Configuration::global->hasGlobalComposite(name)) {

        AbstractComposite *targetComp = Configuration::global->getGlobalComposite(name);
        size = targetComp->getFormat().totalSize;
        targetBase = targetComp;

    } else {

        throw ConfigurationUnknownTargetNameException();

    }


    AbstractTarget target = AbstractTarget(targetBase, 0, size);

    std::set<AbstractTarget> result;
    result.insert(target);

    return result;

}

std::list<const llvm::Instruction *> GlobalTargetFinder::getAssociatedInsts() const {
    return std::list<const llvm::Instruction *>();
}

GlobalTargetFinder::GlobalTargetFinder(const std::string &name) : name(name) {}
