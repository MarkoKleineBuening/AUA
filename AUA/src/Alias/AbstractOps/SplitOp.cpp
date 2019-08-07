//
// Created by mlaupichler on 02.06.19.
//

#include <llvm/Support/raw_ostream.h>
#include "AUA/Alias/AbstractOps/SplitOp.h"

// PROTECTED

Configuration *SplitOp::apply(Configuration *in) {

    return in;

}

// PUBLIC

SplitOp::SplitOp() {

    // Nothing happening

}

void SplitOp::execute(Configuration *in) {

    llvm::outs() << "Splitting.\n";


    this->consume(in);

    for (PointerOperation *successor : succ) {

        Configuration *copyOfInput = in->getDeepCopy();

        successor->execute(copyOfInput);

    }

}

void SplitOp::addPredecessor(PointerOperation *predecessor) {

    if (pred != nullptr) throw PredecessorCountException();

    this->pred = predecessor;

}

void SplitOp::linkSuccessor(PointerOperation *successor) {

    this->succ.push_back(successor);
    successor->addPredecessor(this);

}

std::list<PointerOperation *> SplitOp::getPredecessors() {

    std::list<PointerOperation *> list;
    list.push_back(pred);

    return list;

}

std::list<PointerOperation *> SplitOp::getSuccessors() {

    return succ;

}
