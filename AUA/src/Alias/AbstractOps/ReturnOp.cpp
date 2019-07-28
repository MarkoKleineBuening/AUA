//
// Created by mlaupichler on 28.05.19.
//


#include "AUA/Alias/AbstractOps/ReturnOp.h"


Configuration *ReturnOp::apply(Configuration *in) {

    throw UndefinedPointerOperationMethodException();

}


void ReturnOp::addPredecessor(PointerOperation *predecessor) {

    this->pred = predecessor;

}

void ReturnOp::linkSuccessor(PointerOperation *successor) {

    throw SuccessorCountException(returnInst);

}

std::set<PointerOperation *> ReturnOp::getSuccessors() {

    throw SuccessorCountException(returnInst);

}

std::set<PointerOperation *> ReturnOp::getPredecessors() {

    std::set<PointerOperation *> resultSet;
    resultSet.insert(pred);
    return resultSet;

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
