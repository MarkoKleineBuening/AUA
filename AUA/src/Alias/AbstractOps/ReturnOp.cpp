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

std::list<PointerOperation *> ReturnOp::getSuccessors() {

    throw SuccessorCountException(returnInst);

}

std::list<PointerOperation *> ReturnOp::getPredecessors() {

    std::list<PointerOperation *> list;
    list.push_back(pred);

    return list;

}

Configuration *ReturnOp::getLastConfiguration() {
    return lastConfiguration;
}

void ReturnOp::execute(Configuration *in) {

    llvm::outs() << "Execute on return called.\n";

    lastConfiguration = in;
    setResult(in);

    finished = true;

}

PointerSetValue *ReturnOp::getResult() {

    if (!finished) throw ResultNotReadyException();

    return result;

}

ReturnOp::ReturnOp(llvm::ReturnInst *returnInst) : returnInst(returnInst) {}
