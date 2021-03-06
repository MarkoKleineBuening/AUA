//
// Created by mlaupichler on 02.06.19.
//

#ifndef AUA_SINGLEPREDANDSUCCOP_H
#define AUA_SINGLEPREDANDSUCCOP_H


#include <AUA/Alias/AbstractPointers/Configuration.h>
#include "PointerOperation.h"

/**
 * Abstract class for PointerOperations that have exactly one predecessor and successor. Implements dealing with predecessors and successors and leaves only the actual operation application to the deriving class.
 */
class SinglePredAndSuccOp : public PointerOperation {

protected:

    PointerOperation *pred = nullptr;
    PointerOperation *succ = nullptr;


public:


    void execute(Configuration *in) override;

    void linkSuccessor(PointerOperation *successor) override;

    void addPredecessor(PointerOperation *predecessor) override;

    std::list<PointerOperation *> getPredecessors() override;

    std::list<PointerOperation *> getSuccessors() override;


};


#endif //AUA_SINGLEPREDANDSUCCOP_H
