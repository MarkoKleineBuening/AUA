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

    ++numberOfExecuteCalls;

    if (currentInput == nullptr) {

        assert(numberOfExecuteCalls == 1);
        currentInput = in->getDeepCopy();

    } else {

        assert(numberOfExecuteCalls > 1);
        currentInput->merge(in);
    }


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

std::set<PointerOperation *> JoinOp::getPredecessors() {

    return pred;

}

std::set<PointerOperation *> JoinOp::getSuccessors() {

    std::set<PointerOperation *> resultSet;
    resultSet.insert(succ);

    return resultSet;

}

std::set<llvm::Instruction *> JoinOp::getAssocInstructions() {
    return std::set<llvm::Instruction *>();
}
