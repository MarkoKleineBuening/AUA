//
// Created by mlaupichler on 28.05.19.
//

#include <llvm/Support/raw_ostream.h>
#include "AUA/Alias/AbstractOps/DummyFinalOp.h"

DummyFinalOp::DummyFinalOp() {

}

Configuration* DummyFinalOp::apply(Configuration* in) {

    //TODO Exception werfen

    return nullptr;

}

void DummyFinalOp::execute(Configuration *in) {

    llvm::outs() << "\nResult is: \n";

    in->printFullInfoVerbose();
}

void DummyFinalOp::addPredecessor(PointerOperation *predecessor) {

    this->pred = predecessor;

}

void DummyFinalOp::linkSuccessor(PointerOperation *successor) {

    //TODO Exception werfen

}

std::set<PointerOperation*> DummyFinalOp::getSuccessors() {

    //TODO throw Exception
    return std::set<PointerOperation*>();

}

std::set<PointerOperation*> DummyFinalOp::getPredecessors() {

    std::set<PointerOperation*> resultSet;
    resultSet.insert(pred);
    return resultSet;

}

std::set<llvm::Instruction *> DummyFinalOp::getAssocInstructions() {
    return std::set<llvm::Instruction *>();
}
