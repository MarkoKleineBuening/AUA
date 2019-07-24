//
// Created by mlaupichler on 02.06.19.
//

#include <llvm/Support/raw_ostream.h>
#include "AUA/Alias/AbstractOps/SinglePredAndSuccOp.h"


void SinglePredAndSuccOp::execute(Configuration *in) {


    Configuration *result = this->apply(in);

    this->consume(result);

    succ->execute(result);


}

void SinglePredAndSuccOp::addPredecessor(PointerOperation *predecessor) {

    this->pred = predecessor;

}

void SinglePredAndSuccOp::linkSuccessor(PointerOperation *successor) {

    this->succ = successor;
    successor->addPredecessor(this);

}

std::set<PointerOperation *> SinglePredAndSuccOp::getPredecessors() {

    std::set<PointerOperation *> resultSet;
    resultSet.insert(pred);

    return resultSet;

}

std::set<PointerOperation *> SinglePredAndSuccOp::getSuccessors() {

    std::set<PointerOperation *> resultSet;
    resultSet.insert(succ);

    return resultSet;

}

