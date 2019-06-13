//
// Created by mlaupichler on 02.06.19.
//

#include <llvm/Support/raw_ostream.h>
#include "AUA/Alias/AbstractOps/SplitOp.h"

// PROTECTED

Configuration* SplitOp::apply(Configuration *in) {

    return in;

}

// PUBLIC

SplitOp::SplitOp() {

    // Nothing happening

}

void SplitOp::execute(Configuration *in) {

    llvm::outs() << "Splitting.\n";



    this->consume(in);

    for(PointerOperation* successor : succ) {

        Configuration* copyOfInput = in->getDeepCopy();

        successor->execute(copyOfInput);

    }

}

void SplitOp::addPredecessor(PointerOperation *predecessor) {

    this->pred = predecessor;

}

void SplitOp::linkSuccessor(PointerOperation *successor) {

    this->succ.insert(successor);
    successor->addPredecessor(this);

}

std::set<PointerOperation*> SplitOp::getPredecessors() {

    std::set<PointerOperation*> resultSet;
    resultSet.insert(pred);

    return resultSet;

}

std::set<PointerOperation*> SplitOp::getSuccessors() {

    return succ;

}