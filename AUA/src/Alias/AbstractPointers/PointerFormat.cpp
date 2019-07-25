//
// Created by mlaupichler on 20.07.19.
//

#include <llvm/Support/raw_ostream.h>
#include "AUA/Alias/AbstractPointers/PointerFormat.h"


PointerFormat::PointerFormat(int pointerLevel) : level(pointerLevel) {}

PointerFormat::PointerFormat(llvm::Type *type) : level(getPointerLevel(type)) {}

bool PointerFormat::operator==(const PointerFormat &rhs) const {
    return level == rhs.level;
}

bool PointerFormat::operator!=(const PointerFormat &rhs) const {
    return !(rhs == *this);
}

int PointerFormat::getPointerLevel(llvm::Type *type) {
    int l = 0;

    while (auto ptrType = llvm::dyn_cast<llvm::PointerType>(type)) {

        ++l;
        type = ptrType->getElementType();
    }

    return l;
}

