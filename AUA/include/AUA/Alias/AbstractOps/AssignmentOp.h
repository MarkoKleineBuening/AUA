//
// Created by mlaupichler on 27.05.19.
//

#ifndef AUA_ASSIGNMENTOP_H
#define AUA_ASSIGNMENTOP_H


#include <AUA/Alias/Configuration.h>
#include <llvm/IR/Instructions.h>
#include "SinglePredAndSuccOp.h"

class AssignmentOp : public SinglePredAndSuccOp {

private:

    const std::string pointerName;
    const std::string targetName;
    const llvm::StoreInst* storeInstruction;


protected:

    Configuration* apply(Configuration* in) override;

    std::vector<llvm::Instruction *> getAssocInstructions() override;

public:

    AssignmentOp(std::string ptrName, std::string varName, llvm::StoreInst *storeInst);

};


#endif //AUA_ASSIGNMENTOP_H
