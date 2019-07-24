//
// Created by mlaupichler on 28.05.19.
//


#include "AUA/Alias/AbstractOps/ReturnOp.h"


Configuration *ReturnOp::apply(Configuration *in) {

    //TODO Exception werfen

    return nullptr;

}


void ReturnOp::addPredecessor(PointerOperation *predecessor) {

    this->pred = predecessor;

}

void ReturnOp::linkSuccessor(PointerOperation *successor) {

    //TODO Exception werfen

}

std::set<PointerOperation *> ReturnOp::getSuccessors() {

    //TODO throw Exception
    return std::set<PointerOperation *>();

}

std::set<PointerOperation *> ReturnOp::getPredecessors() {

    std::set<PointerOperation *> resultSet;
    resultSet.insert(pred);
    return resultSet;

}

std::set<llvm::Instruction *> ReturnOp::getAssocInstructions() {
    std::set<llvm::Instruction *> result;
    result.insert(returnInst);
    return result;
}

Configuration *ReturnOp::getLastConfiguration() {
    return lastConfiguration;
}

void ReturnOp::execute(Configuration *in) {

    lastConfiguration = in;
    setResult(in);

    finished = true;

}

PointerSetValue *ReturnOp::getResult() {

    if (!finished) throw ResultNotReadyException();

    return result;

}

ReturnOp::ReturnOp(llvm::ReturnInst *returnInst) : returnInst(returnInst) {}
