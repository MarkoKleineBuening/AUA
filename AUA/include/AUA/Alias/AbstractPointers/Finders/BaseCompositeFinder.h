//
// Created by mlaupichler on 14.07.19.
//

#ifndef AUA_BASECOMPOSITEFINDER_H
#define AUA_BASECOMPOSITEFINDER_H


#include "CompositeFinder.h"
#include "AUA/Alias/AbstractPointers/CompositeFormat.h"

class BaseCompositeFinder : public CompositeFinder {

private:

    const std::string compositeName;

    const llvm::AllocaInst* allocaInst = nullptr;

public:

    /**
     * Constructor for when no llvm::AllocaInst exists for the value associated with this CompositeFinder (e.g. if the value is an input parameter to the function).
     * @param compositeName the name of the composite to find.
     * @param expectedFormat the expected CompositeFormat for the sought AbstractComposite.
     * @param allocaInst the llvm::AllocaInst associated with the value associated with this CompositeFinder.
     */
    BaseCompositeFinder(const std::string& compositeName, const CompositeFormat &expectedFormat,
                        const llvm::AllocaInst *allocaInst);

    /**
     * Constructor for when no llvm::AllocaInst exists for the value this composite represents (e.g. if the value is an input parameter to the function).
     * @param compositeName the name of the commposite to find.
     * @param expectedFormat the expected CompositeFormat for the sought composite.
     */
    BaseCompositeFinder(const std::string& compositeName, const CompositeFormat &expectedFormat);

    CompositeSetValue *findComposites(Configuration *configuration) const override;

    std::list<const llvm::Instruction *> getAssociatedInsts() const override;

};


#endif //AUA_BASECOMPOSITEFINDER_H
