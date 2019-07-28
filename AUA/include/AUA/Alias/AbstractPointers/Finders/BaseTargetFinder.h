//
// Created by mlaupichler on 22.06.19.
//

#ifndef AUA_BASETARGETFINDER_H
#define AUA_BASETARGETFINDER_H


#include "TargetFinder.h"

class BaseTargetFinder : public TargetFinder {

private:

    const std::string targetName;

    const llvm::AllocaInst* allocaInst = nullptr;

public:

    /**
     * Constructor for when no llvm::AllocaInst exists for the value associated with this TargetFinder (e.g. if the value is an input parameter to the function).
     * @param targetName the name of the target to find.
     */
    explicit BaseTargetFinder(const std::string &targetName);

    /**
     * Constructor for when an llvm::AllocaInst exists for the value associated with this TargetFinder.
     * @param targetName the name of the target to find.
     * @param allocaInst the llvm::AllocaInst associated with the value associated with this TargetFinder.
     */
    explicit BaseTargetFinder(std::string targetName, const llvm::AllocaInst *allocaInst);

    std::set<AbstractTarget> findTargets(Configuration *conf) const override;

    std::list<const llvm::Instruction *> getAssociatedInsts() const override;

};


#endif //AUA_BASETARGETFINDER_H
