//
// Created by mlaupichler on 12.05.19.
//

#ifndef AUA_ABSTRACTPOINTER_H
#define AUA_ABSTRACTPOINTER_H

#include <string>
#include <unordered_set>
#include <set>
#include <list>
#include <llvm/IR/Instruction.h>
#include <llvm/IR/DerivedTypes.h>
#include <bits/unordered_set.h>
#include "AbstractVar.h"
#include "AbstractTarget.h"
#include "PointerFormat.h"


class AbstractPointer : public AbstractReference {

private:

    PointerFormat format;

    std::set<AbstractTarget> targets;
    std::unordered_set<const llvm::Instruction *> assocInsts;

public:

    /**
     * Constructor for a pointer with given name and format. Initially the pointer will not reference any targets. The AbstractPointer will have default ReferenceFlags.
     * @param n the name of the pointer.
     * @param format the level of the pointer.
     */
    AbstractPointer(std::string n, PointerFormat format);

    /**
     * Constructor for a pointer with given name, format and reference flags. Initially the pointer will not reference any targets.
     * @param n the name of the pointer.
     * @param format the level of the pointer.
     * @param flags the ReferenceFlags for the constructed pointer.
     */
    AbstractPointer(std::string n, PointerFormat format, ReferenceFlags flags);

    const int getPointerLevel() override { return format.level; };

    const std::string to_string() override;

    const PointerFormat &getFormat() const;

    std::set<AbstractTarget> getTargets() { return targets; };

    /**
     * Calculates all pointers this pointer points to at (this pointers level - derefDepth)and returns their targets.
     * @param derefDepth the number of times this pointer is virtually dereferenced to get the lower level pointers.
     * @return the targets of the lower level pointers.
     */
    std::set<AbstractTarget> derefAndGetTargets(int derefDepth);

    /**
     * Calculates all pointers this pointer points to at (this pointers level - derefDepth) and returns their targets.
     * @param derefDepth the number of times this pointer is virtually dereferenced to get the lower level pointers.
     * @param associatedInsts all associated instructions of pointers passed in dereferencing this pointer are added to the given set of instructions.
     * @return the targets of the lower level pointers.
     */
    std::set<AbstractTarget> derefAndGetTargets(int derefDepth, std::list<const llvm::Instruction *> *associatedInsts);

    AbstractPointer *getCopy();


    bool operator<(const AbstractPointer &other) const { return name < other.name; };

    bool operator==(const AbstractPointer &other) const { return name == other.name; };

    /**
     * Makes the pointer point exclusively to the given target. All previous pointers to targets are removed.
     * @param target the target to make this Pointer exclusively point to.
     */
    void onlyPointTo(AbstractTarget target);

    /**
     * Adds a pointer to a target to this Pointer. Keeps previous pointers to targets.
     * @param target the target to add a pointer to.
     */
    void alsoPointTo(AbstractTarget target);

    /**
     * Drops all previous pointers to targets from this Pointer and copies the pointers from another given Pointer.
     * @param other the Pointer to copy all pointers to targets from.
     */
    void copyTargetsFrom(AbstractPointer *other);

    void setTargets(const std::set<AbstractTarget> &set);

    void addTargets(std::set<AbstractTarget> targetsToAdd);


    void merge(AbstractPointer *other);

    void setAssocInsts(const std::list<const llvm::Instruction *> &newAssocInsts);

    void setOnlyAssocInst(llvm::Instruction *assocInst);

    void addAssocInst(llvm::Instruction *inst);

    void addAllAssocInsts(std::list<const llvm::Instruction *> insts);

    std::unordered_set<const llvm::Instruction *> getAssociatedInsts();
};

struct DerefPointerLevelException : public std::exception {};


#endif //AUA_ABSTRACTPOINTER_H
