//
// Created by mlaupichler on 02.06.19.
//

#include <llvm/Support/raw_ostream.h>
#include "AUA/Alias/AbstractOps/JoinOp.h"

// PROTECTED

Configuration *JoinOp::apply(Configuration *in) {

    return in;

}


// PUBLIC

JoinOp::JoinOp() : currentInput(nullptr) {

    this->numberOfExecuteCalls = 0;

}

void JoinOp::execute(Configuration *in) {

    llvm::outs() << "JoinOp reached.\n";

    ++numberOfExecuteCalls;

    if (currentInput == nullptr) {

        assert(numberOfExecuteCalls == 1);
        currentInput = in->getDeepCopy();

    } else {

        assert(numberOfExecuteCalls > 1);
        currentInput->merge(in);
    }

    llvm::outs() << "Call number :" << numberOfExecuteCalls <<"\n";
    llvm::outs() << "Predecessor count :" << pred.size() <<"\n";

    if (numberOfExecuteCalls < pred.size()) {

        llvm::outs() << "Not going on yet because call number < pred count.\n";
        return;
    }

    llvm::outs() << "\nJoining.";

    //Configuration* result = this->apply(currentInput);

    this->consume(currentInput);

    Configuration* result = currentInput;

    resetState();

    succ->execute(result);

}

void JoinOp::addPredecessor(PointerOperation *predecessor) {

    this->pred.push_back(predecessor);

}

void JoinOp::linkSuccessor(PointerOperation *successor) {

    this->succ = successor;
    successor->addPredecessor(successor);

}

std::list<PointerOperation *> JoinOp::getPredecessors() {

    return pred;

}

std::list<PointerOperation *> JoinOp::getSuccessors() {

    std::list<PointerOperation *> resultList;
    resultList.push_back(succ);

    return resultList;

}

void JoinOp::resetState() {

    numberOfExecuteCalls = 0;
    currentInput = nullptr;

}
