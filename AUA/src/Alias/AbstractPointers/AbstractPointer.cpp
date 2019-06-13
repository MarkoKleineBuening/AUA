//
// Created by mlaupichler on 12.05.19.
//

#include <string>
#include <AUA/Alias/AbstractPointers/AbstractPointer.h>
#include <llvm/Support/FileSystem.h>

/**
 * Constructor for a pointer with given alignment and alphanumerical name. Initially the pointer will not reference any VarRefs.
 * @param a the alignment of the pointer.
 * @param n the name of the pointer.
 */
AbstractPointer::AbstractPointer(int a, std::string n) {
    alignment = a;
    name = n;
}

/**
 * Makes the pointer point exclusively to the given VarRef. All previous pointers to VarRefs are removed.
 * @param var the VarRef to make this Pointer exclusively point to.
 */
void AbstractPointer::onlyPointTo(AbstractTarget var) {
    targets.clear();
    targets.insert(var);
}

/**
 * Adds a pointer to a VarRef to this Pointer. Keeps previous pointers to VarRefs.
 * @param var the VarRef to add a pointer to.
 */
void AbstractPointer::alsoPointTo(AbstractTarget var) {
    targets.insert(var);
}

/**
 * Drops all previous pointers to VarRefs from this Pointer and copies the pointers from another given Pointer.
 * @param other the Pointer to copy all pointers to VarRefs from.
 */
void AbstractPointer::copyTargetsFrom(AbstractPointer *other) {
    targets = other->targets;
}

void AbstractPointer::mergeTargets(AbstractPointer *other) {

//    for (AbstractTarget target : other->getTargets()) {
//
//        for (auto target2 : this->targets) {
//
//            llvm::outs() << target.toString() << " < " << target2.toString()   << " : " << (target < target2) << "\n";
//
//        }
//    }

    targets.merge(other->getTargets());


}

AbstractPointer* AbstractPointer::getCopy() {

        AbstractPointer* copy = new AbstractPointer(this->alignment, this->name);
        copy->copyTargetsFrom(this);

        return copy;

}




