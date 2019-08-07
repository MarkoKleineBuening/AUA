//
// Created by mlaupichler on 02.06.19.
//

#ifndef AUA_SPLITOP_H
#define AUA_SPLITOP_H


#include "PointerOperation.h"

class SplitOp : public PointerOperation {

protected:

    PointerOperation *pred = nullptr;
    std::list<PointerOperation *> succ;

    Configuration *apply(Configuration *in) override;


public:

    SplitOp();

    void execute(Configuration *in) override;

    void linkSuccessor(PointerOperation *successor) override;

    void addPredecessor(PointerOperation *predecessor) override;

    std::list<PointerOperation *> getPredecessors() override;

    std::list<PointerOperation *> getSuccessors() override;

};


#endif //AUA_SPLITOP_H
