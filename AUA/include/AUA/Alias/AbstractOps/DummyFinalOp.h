//
// Created by mlaupichler on 28.05.19.
//

#ifndef AUA_DUMMYFINALOP_H
#define AUA_DUMMYFINALOP_H


#include <AUA/Alias/Configuration.h>
#include "SinglePredAndSuccOp.h"

class DummyFinalOp : public PointerOperation {

private:

    PointerOperation* pred;

protected:

    Configuration* apply(Configuration* in) override;

    std::vector<llvm::Instruction *> getAssocInstructions() override;

public:

    DummyFinalOp();

    void execute(Configuration* in) override;

    void linkSuccessor(PointerOperation* successor) override;
    void addPredecessor(PointerOperation *predecessor) override;

    std::set<PointerOperation*> getPredecessors() override;
    std::set<PointerOperation*> getSuccessors() override;

};


#endif //AUA_DUMMYFINALOP_H
