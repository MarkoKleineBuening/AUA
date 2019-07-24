//
// Created by mlaupichler on 20.07.19.
//

#include <llvm/Support/raw_ostream.h>
#include "AUA/Alias/AbstractPointers/PointerFormat.h"


PointerFormat::PointerFormat(int pointerLevel) : level(pointerLevel) {}

PointerFormat::PointerFormat(llvm::PointerType *ptrType) : level(getPointerLevel(ptrType)) {}

bool PointerFormat::operator==(const PointerFormat &rhs) const {
    return level == rhs.level;
}

bool PointerFormat::operator!=(const PointerFormat &rhs) const {
    return !(rhs == *this);
}

int PointerFormat::getPointerLevel(llvm::PointerType *ptrType) {
    int l = 1;
    llvm::Type *elType = ptrType->getElementType();
    while ((ptrType = llvm::dyn_cast<llvm::PointerType>(elType))) {

        ++l;
        elType = ptrType->getElementType();
    }

    return l;
}

//PointerFormat::PointerFormat() : level(-1) {}
