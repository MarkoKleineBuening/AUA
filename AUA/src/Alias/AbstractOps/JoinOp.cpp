//
// Created by mlaupichler on 02.06.19.
//

#include <llvm/Support/raw_ostream.h>
#include "AUA/Alias/AbstractOps/JoinOp.h"

// PROTECTED

Configuration* JoinOp::apply(Configuration *in) {

    return in;

}


// PUBLIC

JoinOp::JoinOp() {

    this->currentInput = new Configuration();
    this->numberOfExecuteCalls = 0;

}

void JoinOp::execute(Configuration* in) {

    ++numberOfExecuteCalls;
    currentInput->merge(in);

    if (numberOfExecuteCalls < pred.size()) return;

    llvm::outs() << "\nJoining.";

    //Configuration* result = this->apply(currentInput);

    this->consume(currentInput);

    succ->execute(currentInput);

}

void JoinOp::addPredecessor(PointerOperation *predecessor) {

    this->pred.insert(predecessor);

}

void JoinOp::linkSuccessor(PointerOperation *successor) {

    this->succ = successor;
    successor->addPredecessor(successor);

}

std::set<PointerOperation*> JoinOp::getPredecessors() {

    return pred;

}

std::set<PointerOperation*> JoinOp::getSuccessors() {

    std::set<PointerOperation*> resultSet;
    resultSet.insert(succ);

    return resultSet;

}