//
// Created by mlaupichler on 12.05.19.
//

#include <AUA/Alias/Pointer.h>


/**
 * Constructor for a pointer with given alignment and alphanumerical name. Initially the pointer will not reference any VarRefs.
 * @param a the alignment of the pointer.
 * @param n the name of the pointer.
 */
Pointer::Pointer(int a, std::string n) {
    alignment = a;
    name = n;
}

/**
 * Makes the pointer point exclusively to the given VarRef. All previous pointers to VarRefs are removed.
 * @param var the VarRef to make this Pointer exclusively point to.
 */
void Pointer::onlyPointTo(VarRef *var) {
    varRefs.clear();
    varRefs.insert(var);
}

/**
 * Adds a pointer to a VarRef to this Pointer. Keeps previous pointers to VarRefs.
 * @param var the VarRef to add a pointer to.
 */
void Pointer::alsoPointTo(VarRef *var) {
    varRefs.insert(var);
}

/**
 * Drops all previous pointers to VarRefs from this Pointer and copies the pointers from another given Pointer.
 * @param other the Pointer to copy all pointers to VarRefs from.
 */
void Pointer::copyPointersFrom(Pointer *other) {
    varRefs = other->varRefs;
}




