//
// Created by mlaupichler on 02.06.19.
//

#ifndef AUA_JOINOP_H
#define AUA_JOINOP_H


#include "PointerOperation.h"

class JoinOp : public PointerOperation {

private:

    int numberOfExecuteCalls;
    Configuration *currentInput;

protected:

    std::set<PointerOperation *> pred;
    PointerOperation *succ;

    Configuration *apply(Configuration *in) override;

    std::set<llvm::Instruction *> getAssocInstructions() override;

public:

    JoinOp();

    void execute(Configuration *in) override;

    void linkSuccessor(PointerOperation *successor) override;

    void addPredecessor(PointerOperation *predecessor) override;

    std::set<PointerOperation *> getPredecessors() override;

    std::set<PointerOperation *> getSuccessors() override;

};


#endif //AUA_JOINOP_H
