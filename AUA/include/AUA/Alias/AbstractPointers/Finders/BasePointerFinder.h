//
// Created by mlaupichler on 22.06.19.
//

#ifndef AUA_BASEPOINTERFINDER_H
#define AUA_BASEPOINTERFINDER_H


#include <AUA/Alias/AbstractPointers/SetValues/PointerSetValue.h>
#include "PointerFinder.h"

class BasePointerFinder : public PointerFinder {

private:

    const std::string pointerName;

    const llvm::AllocaInst* allocaInst = nullptr;

public:

    /**
     * Constructor for when no llvm::AllocaInst exists for the value associated with this PointerFinder (e.g. if the value is an input parameter to the function).
     * @param pointerName the name of the pointer to find.
     * @param expectedFormat the expected PointerFormat for the sought pointer.
     */
    BasePointerFinder(std::string pointerName, const PointerFormat &expectedFormat);

    /**
     * Constructor for when an explicit llvm::AllocaInst exists for the value associated with this PointerFinder .
     * @param pointerName the name of the pointer to find.
     * @param expectedFormat the expected PointerFormat for the sought pointer.
     * @param allocaInst the llvm::AllocaInst associated with the value associated with this PointerFinder .
     */
    BasePointerFinder(std::string pointerName, const PointerFormat &expectedFormat,
                      const llvm::AllocaInst *allocaInst);

    PointerSetValue *findPointers(Configuration *configuration) const override;

    std::list<const llvm::Instruction *> getAssociatedInsts() const override;

};


#endif //AUA_BASEPOINTERFINDER_H
