//
// Created by mlaupichler on 12.05.19.
//

#include <string>
#include <AUA/Alias/AbstractPointers/AbstractPointer.h>
#include <llvm/Support/FileSystem.h>


AbstractPointer::AbstractPointer(std::string n, int a, int l) : AbstractReference(n, a), level(l) {}

/**
 * Makes the pointer point exclusively to the given target. All previous pointers to targets are removed.
 * @param target the target to make this Pointer exclusively point to.
 */
void AbstractPointer::onlyPointTo(AbstractTarget target) {

    assert(target.base->getLevel() == this->level - 1);

    targets.clear();
    targets.insert(target);
}

/**
 * Adds a pointer to a target to this Pointer. Keeps previous pointers to targets.
 * @param target the target to add a pointer to.
 */
void AbstractPointer::alsoPointTo(AbstractTarget target) {

    assert(target.base->getLevel() == this->level - 1);

    targets.insert(target);
}

/**
 * Drops all previous pointers to targets from this Pointer and copies the pointers from another given Pointer.
 * @param other the Pointer to copy all pointers to targets from.
 */
void AbstractPointer::copyTargetsFrom(AbstractPointer *other) {

    assert(this->level == other->level);

    targets = other->targets;
}

void AbstractPointer::mergeTargets(AbstractPointer *other) {

    assert(this->level == other->level);

    targets.merge(other->getTargets());


}

AbstractPointer* AbstractPointer::getCopy() {

        AbstractPointer* copy = new AbstractPointer(this->name, this->alignment, this->level);
        copy->copyTargetsFrom(this);

        return copy;

}

void AbstractPointer::setTargets(std::set<AbstractTarget> newTargets) {

    for (auto target : newTargets) {
        assert(target.base->getLevel() == this->level - 1);
    }

    this->targets = newTargets;

}




