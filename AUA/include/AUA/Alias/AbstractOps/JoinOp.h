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

    void resetState();

protected:

    std::list<PointerOperation *> pred;
    PointerOperation *succ;

    Configuration *apply(Configuration *in) override;

public:

    JoinOp();

    void execute(Configuration *in) override;

    void addPredecessor(PointerOperation *predecessor) override;

    void linkSuccessor(PointerOperation *successor) override;

    std::list<PointerOperation *> getPredecessors() override;

    std::list<PointerOperation *> getSuccessors() override;

};


#endif //AUA_JOINOP_H
