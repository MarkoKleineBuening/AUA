//
// Created by mlaupichler on 27.05.19.
//

#ifndef AUA_ASSIGNMENTOP_H
#define AUA_ASSIGNMENTOP_H


#include <AUA/Alias/AbstractPointers/Configuration.h>
#include <llvm/IR/Instructions.h>
#include <AUA/Alias/AbstractPointers/Finders/PointerFinder.h>
#include <AUA/Alias/AbstractPointers/Finders/TargetFinder.h>
#include "SinglePredAndSuccOp.h"

class AssignmentOp : public SinglePredAndSuccOp {

private:

    const PointerFinder* pointerFinder;
    const TargetFinder* targetFinder;

    const llvm::StoreInst* storeInstruction;

protected:

    Configuration* apply(Configuration* in) override;

    std::set<llvm::Instruction *> getAssocInstructions() override;

public:

    /**
     * Constructor
     * @param pointerFinder the PointerFinder that is parametrized with the pointer data for this assignment. Used to find the according pointer later.
     * @param targetFinder the TargetFinder that is parametrized with the target data for this assignment. Used to build the according target later.
     * @param storeInstruction the llvm::StoreInst associated with this assignment
     */
    AssignmentOp(const PointerFinder *pointerFinder, const TargetFinder *targetFinder,
                 const llvm::StoreInst *storeInstruction);


};


#endif //AUA_ASSIGNMENTOP_H
