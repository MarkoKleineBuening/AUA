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

    if (pred != nullptr) throw PredecessorCountException();

    this->pred = predecessor;

}

void SinglePredAndSuccOp::linkSuccessor(PointerOperation *successor) {

    if (succ != nullptr) throw SuccessorCountException();

    this->succ = successor;
    successor->addPredecessor(this);

}

std::list<PointerOperation *> SinglePredAndSuccOp::getPredecessors() {

    std::list<PointerOperation *> list;
    list.push_back(pred);

    return list;

}

std::list<PointerOperation *> SinglePredAndSuccOp::getSuccessors() {

    std::list<PointerOperation *> list;
    list.push_back(succ);

    return list;

}

