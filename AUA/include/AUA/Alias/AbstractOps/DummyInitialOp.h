//
// Created by mlaupichler on 05.06.19.
//

#ifndef AUA_DUMMYINITIALOP_H
#define AUA_DUMMYINITIALOP_H


#include <AUA/Alias/Configuration.h>
#include "PointerOperation.h"

class DummyInitialOp : public PointerOperation {

private:

    PointerOperation* succ;

protected:

    Configuration* apply(Configuration* in) override;

public:

    DummyInitialOp();

    void execute(Configuration* in) override;

    void linkSuccessor(PointerOperation* successor) override;
    void addPredecessor(PointerOperation *predecessor) override;

    std::set<PointerOperation*> getPredecessors() override;
    std::set<PointerOperation*> getSuccessors() override;

};


#endif //AUA_DUMMYINITIALOP_H