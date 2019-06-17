//
// Created by mlaupichler on 27.05.19.
//

#ifndef AUA_COPYOP_H
#define AUA_COPYOP_H


#include <AUA/Alias/AbstractPointers/Configuration.h>
#include <llvm/IR/Instructions.h>
#include "SinglePredAndSuccOp.h"

class CopyOp : public SinglePredAndSuccOp {

private:
    const std::string fromName;
    const std::string toName;
    const int derefDepth;
    const llvm::StoreInst* storeInstruction;
    const std::list<llvm::LoadInst*> loadInstructions;

protected:

    Configuration* apply(Configuration* in) override;

    std::set<llvm::Instruction *> getAssocInstructions() override;

public:

    CopyOp(std::string fromName, std::string toName, int derefDepth, llvm::StoreInst *storeInst,
           const std::list<llvm::LoadInst*>);
};


#endif //AUA_COPYOP_H
