//
// Created by mlaupichler on 02.06.19.
//

#ifndef AUA_SPLITOP_H
#define AUA_SPLITOP_H


#include "PointerOperation.h"

class SplitOp : public PointerOperation {

protected:

    PointerOperation *pred;
    std::set<PointerOperation *> succ;

    Configuration *apply(Configuration *in) override;


public:

    SplitOp();

    void execute(Configuration *in) override;

    void linkSuccessor(PointerOperation *successor) override;

    void addPredecessor(PointerOperation *predecessor) override;

    std::set<PointerOperation *> getPredecessors() override;

    std::set<PointerOperation *> getSuccessors() override;

};


#endif //AUA_SPLITOP_H
