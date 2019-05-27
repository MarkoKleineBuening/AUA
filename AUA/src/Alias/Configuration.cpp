//
// Created by mlaupichler on 27.05.19.
//

#include "AUA/Alias/Configuration.h"

#include <llvm/Support/FileSystem.h>

/**
 * TODO
 * @param other
 */
void Configuration::merge(Configuration &other) {
    return;
}

void Configuration::printFullInfo() {

    printPointerInfo();
    printVarInfo();
}

void Configuration::printPointerInfo() {

    llvm::outs() << "\n" << pointers.size() << " Pointers:\n";
    for(std::pair<std::string, AbstractPointer*> p : pointers) {
        llvm::outs() << p.first << " points to: ";
        for(AbstractTarget* t : p.second->getTargets()) {
            llvm::outs() << "(" << t->base->getName() << ", " << t->byteOffset << ", " << t->size << ") ";
        }
        llvm::outs() << "\n";
    }

    llvm::outs() << "\n";
}

void Configuration::printVarInfo() {

    llvm::outs() << "\n" << vars.size() << " variables:\n";
    for(std::pair<std::string, VarRef*> v : vars) {
        llvm::outs() << v.first << "\n";
    }
    llvm::outs() << "\n";
}