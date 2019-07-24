//
// Created by mlaupichler on 05.06.19.
//

#include <llvm/Support/raw_ostream.h>
#include <AUA/Alias/AbstractOps/DummyInitialOp.h>

DummyInitialOp::DummyInitialOp() {

}

Configuration *DummyInitialOp::apply(Configuration *in) {

    //TODO Exception werfen

    return nullptr;

}

void DummyInitialOp::execute(Configuration *in) {

    llvm::outs() << "\n\nExecuting PointerOperations.\n\n";
    this->succ->execute(in);

}

void DummyInitialOp::addPredecessor(PointerOperation *predecessor) {

    //TODO Exception werfen

}

void DummyInitialOp::linkSuccessor(PointerOperation *successor) {

    this->succ = successor;
    successor->addPredecessor(this);

}

std::set<PointerOperation *> DummyInitialOp::getSuccessors() {

    std::set<PointerOperation *> resultSet;
    resultSet.insert(succ);
    return resultSet;

}

std::set<PointerOperation *> DummyInitialOp::getPredecessors() {


    //TODO throw Exception
    return std::set<PointerOperation *>();

}

std::set<llvm::Instruction *> DummyInitialOp::getAssocInstructions() {
    return std::set<llvm::Instruction *>();
}
