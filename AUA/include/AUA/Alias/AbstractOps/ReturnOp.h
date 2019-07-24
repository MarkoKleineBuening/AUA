//
// Created by mlaupichler on 28.05.19.
//

#ifndef AUA_RETURNOP_H
#define AUA_RETURNOP_H

#include <AUA/Alias/AbstractOps/PointerOperation.h>
#include <llvm/Support/raw_ostream.h>
#include <AUA/Alias/AbstractPointers/SetValues/SetValue.h>
#include <AUA/Alias/AbstractPointers/SetValues/PointerSetValue.h>
#include <llvm/IR/Instructions.h>

class ReturnOp : public PointerOperation {

private:

    PointerOperation *pred;
    bool finished = false;

protected:


    Configuration *lastConfiguration;
    PointerSetValue *result;

    llvm::ReturnInst *returnInst;

    Configuration *apply(Configuration *in) override;

    std::set<llvm::Instruction *> getAssocInstructions() override;

    virtual void setResult(Configuration *in) = 0;

public:


    explicit ReturnOp(llvm::ReturnInst *returnInst);

    void execute(Configuration *in) override;

    void linkSuccessor(PointerOperation *successor) override;

    void addPredecessor(PointerOperation *predecessor) override;

    std::set<PointerOperation *> getPredecessors() override;

    std::set<PointerOperation *> getSuccessors() override;

    Configuration *getLastConfiguration();


    PointerSetValue *getResult();
};

class ResultNotReadyException : public std::exception {

};


#endif //AUA_RETURNOP_H
