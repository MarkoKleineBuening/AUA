//
// Created by mlaupichler on 27.05.19.
//

#include "AUA/Alias/Configuration.h"

#include <llvm/Support/FileSystem.h>

/**
 * Merges this Configuration with another given Configuration. Afterwards this Configuration holds all pointers and variables from both Configurations and all pointers have the targets from both configurations.
 * @param other the Configuration to merge with.
 */
void Configuration::merge(Configuration* other) {

    // merge pointers

    for(auto pointerEntry : other->pointers) {

        if (this->pointers.find(pointerEntry.first) == this->pointers.end()) {
            this->pointers.insert(pointerEntry);
        } else {

            std::string pointerName = pointerEntry.first;

            AbstractPointer* pointerInThis = this->pointers.at(pointerName);
            AbstractPointer* pointerInOther = pointerEntry.second;


            pointerInThis->mergeTargets(pointerInOther);

        }
    }

    //merge vars

    this->vars.merge(other->vars);

}

Configuration* Configuration::getDeepCopy() {

    Configuration* copy = new Configuration();

    // Get actual copies of pointers
    for(auto pointerEntry : this->pointers) {
        AbstractPointer* ptrCopy = pointerEntry.second->getCopy();
        copy->pointers[pointerEntry.first] =  ptrCopy;

    }

    // Variables are constant, and as such don't need to be actually copied
    copy->vars = this->vars;

    return copy;

}


void Configuration::printFullInfo() {

    printPointerInfo();
    printVarInfo();
}

void Configuration::printPointerInfo() {

    llvm::outs() << "\n" << pointers.size() << " Pointers:\n";
    for(std::pair<std::string, AbstractPointer*> p : pointers) {
        llvm::outs() << p.first << " points to: ";
        for(AbstractTarget t : p.second->getTargets()) {
            llvm::outs() << t.toString() << " ";
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