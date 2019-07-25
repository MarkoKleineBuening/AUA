//
// Created by mlaupichler on 20.07.19.
//

#ifndef AUA_POINTERFORMAT_H
#define AUA_POINTERFORMAT_H


#include <llvm/IR/DerivedTypes.h>

struct PointerFormat {

    const int level;

    explicit PointerFormat(llvm::Type *type);

    explicit PointerFormat(int pointerLevel);

    bool operator==(const PointerFormat &rhs) const;

    bool operator!=(const PointerFormat &rhs) const;

private:

    static int getPointerLevel(llvm::Type *type);
};


#endif //AUA_POINTERFORMAT_H
