//
// Created by mlaupichler on 12.05.19.
//

#ifndef AUA_ABSTRACTPOINTER_H
#define AUA_ABSTRACTPOINTER_H

#include <string>
#include <set>
#include <llvm/IR/Instruction.h>
#include "VarRef.h"
#include "AbstractTarget.h"

class AbstractPointer : public AbstractReference {

private:

    const int level;
    std::set<AbstractTarget> targets;
    std::set<llvm::Instruction*> assocInsts;

public:

    /**
     * Constructor for a pointer with given alignment, alphanumerical name and level. Initially the pointer will not reference any targets.
     * @param a the alignment of the pointer.
     * @param n the name of the pointer.
     * @param l the level of the pointer.
     */
    AbstractPointer(std::string n, int a, int l);

    const int getLevel() override {return level;};


    std::set<AbstractTarget> getTargets(){return targets;};

    AbstractPointer* getCopy();


    bool operator <(const AbstractPointer & other) const {return name < other.name;};
    bool operator==(const AbstractPointer & other) const {return name == other.name;};
    void onlyPointTo(AbstractTarget target);

    void alsoPointTo(AbstractTarget target);
    void copyTargetsFrom(AbstractPointer *other);
    void setTargets(std::set<AbstractTarget> set);

    void merge(AbstractPointer *other);


    void setAssocInsts(const std::set<llvm::Instruction *> &assocInsts);
    void setOnlyAssocInst(llvm::Instruction* assocInst);
    void addAssocInst( llvm::Instruction* inst);
    void addAllAssocInsts(std::set<llvm::Instruction*> insts);
    std::set<llvm::Instruction *> getAssocInsts();
};




#endif //AUA_ABSTRACTPOINTER_H
