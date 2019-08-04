//
// Created by mlaupichler on 28.07.19.
//

#include <set>
#include <AUA/Alias/AbstractPointers/Configuration.h>
#include "AUA/Alias/AbstractPointers/Finders/GlobalTargetFinder.h"

std::set <AbstractTarget> GlobalTargetFinder::findTargets(Configuration *conf) const {

    AbstractReference *targetBase = Configuration::global->getGlobalReference(name);
    int size = targetBase->getSize();

    AbstractTarget target = AbstractTarget(targetBase, 0, size);

    std::set<AbstractTarget> result;
    result.insert(target);

    return result;

}

std::list<const llvm::Instruction *> GlobalTargetFinder::getAssociatedInsts() const {
    return std::list<const llvm::Instruction *>();
}

GlobalTargetFinder::GlobalTargetFinder(const std::string &name) : name(name) {}
