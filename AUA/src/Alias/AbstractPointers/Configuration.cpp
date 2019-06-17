//
// Created by mlaupichler on 27.05.19.
//

#include "AUA/Alias/AbstractPointers/Configuration.h"

#include <llvm/Support/FileSystem.h>

// PRIVATE

std::list<Alias> Configuration::calculateAliases() {

    std::list<Alias> result;

    for(auto pointerEntry : pointers) {

        auto primaryPointer = pointerEntry.second;

        for (auto otherPointerEntry : pointers) {

            auto otherPointer = otherPointerEntry.second;
            if (primaryPointer == otherPointer || primaryPointer->getLevel() != otherPointer->getLevel()) {
                continue;
            }

            for (AbstractTarget targetInPrimary : primaryPointer->getTargets()) {
                for (AbstractTarget targetInOther: otherPointer->getTargets()) {

                    if (targetInPrimary == targetInOther) {
                        Alias aliasFound = Alias(primaryPointer, otherPointer, targetInPrimary);

                        bool alreadyExists = false;
                        for (auto existingAlias : result) {

                            if (aliasFound == existingAlias) {
                                alreadyExists = true;
                                break;
                            }
                        }

                        if (!alreadyExists) {
                            result.push_back(aliasFound);
                        }

                    }

                }
            }

        }

    }

    return result;
}


// PUBLIC

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


            pointerInThis->merge(pointerInOther);

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
        llvm::outs() << p.first << " (level " << p.second->getLevel() << ") points to: ";
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

void Configuration::printAliasInfo() {

    auto aliases = calculateAliases();

    llvm::outs() << "\nThe potential aliases are: \n";
    for(auto alias : aliases) {

        llvm::outs() << "\n" << alias.to_String();

    }
    llvm::outs() << "\n";

}

void Configuration::printFullInfoVerbose() {

    printAliasInfo();
    printPointerInfoVerbose();
    printVarInfo();

}

void Configuration::printPointerInfoVerbose() {

    llvm::outs() << "\n" << pointers.size() << " Pointers:\n";
    for(std::pair<std::string, AbstractPointer*> p : pointers) {
        llvm::outs() << "\n" << p.first << " (level " << p.second->getLevel() << ") points to: ";

        for(AbstractTarget t : p.second->getTargets()) {
            llvm::outs() << t.toString() << " ";
        }

        llvm::outs() << "\n" << p.second->getName() << " was influenced by the following instructions: \n";

        for (llvm::Instruction* assocInst: p.second->getAssocInsts()) {
            llvm::outs() << *assocInst << "\n";
        }

        llvm::outs() << "\n\n";
    }

}

