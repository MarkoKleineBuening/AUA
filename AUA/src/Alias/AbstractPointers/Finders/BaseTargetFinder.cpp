#include <utility>

//
// Created by mlaupichler on 22.06.19.
//

#include <llvm/IR/Instructions.h>
#include "AUA/Alias/AbstractPointers/Finders/BaseTargetFinder.h"

BaseTargetFinder::BaseTargetFinder(const std::string &targetName)
        : BaseTargetFinder(targetName, nullptr) {}

BaseTargetFinder::BaseTargetFinder(const std::string targetName, const llvm::AllocaInst *allocaInst)
        : targetName(targetName), allocaInst(allocaInst) {}

std::set<AbstractTarget> BaseTargetFinder::findTargets(Configuration *conf) const {

    AbstractReference *targetBase;
    int size;

    if (conf->vars.find(targetName) != conf->vars.end()) {

        targetBase = conf->vars[targetName];
        size = targetBase->getSize();

    } else if (conf->pointers.find(targetName) != conf->pointers.end()) {

        targetBase = conf->pointers[targetName];
        size = targetBase->getSize();

    } else if (conf->composites.find(targetName) != conf->composites.end()) {

        targetBase = conf->composites[targetName];
        size = targetBase->getSize();

    } else {

        throw ConfigurationUnknownTargetNameException();
    }


    AbstractTarget target = AbstractTarget(targetBase, 0, size);

    std::set<AbstractTarget> result;
    result.insert(target);

    return result;

}

std::list<const llvm::Instruction *> BaseTargetFinder::getAssociatedInsts() const {

    auto result = std::list<const llvm::Instruction*>();
    if (allocaInst != nullptr)
        result.push_back(allocaInst);

    return result;

}
