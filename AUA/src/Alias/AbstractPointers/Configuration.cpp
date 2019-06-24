//
// Created by mlaupichler on 27.05.19.
//

#include "AUA/Alias/AbstractPointers/Configuration.h"

#include <llvm/Support/FileSystem.h>

// PRIVATE

std::list<Alias> Configuration::calculateAliases() {

    std::list<Alias> result;

    std::set<AbstractPointer*> allPointers = getAllPointers();

    for(auto primaryPointer : allPointers) {

        for (auto otherPointer : allPointers) {

            if (primaryPointer == otherPointer || primaryPointer->getPointerLevel() != otherPointer->getPointerLevel()) {
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
 * Merges this Configuration with another given Configuration.
 * Afterwards this Configuration holds all pointers and variables from both Configurations
 * and all pointers have the targets from both configurations.
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
    printCompositeInfo();
}

void Configuration::printPointerInfo() {

    std::set<AbstractPointer*> allPointers = getAllPointers();

    llvm::outs() << "\n" << allPointers.size() << " Pointers:\n";
    for(auto p : allPointers) {
        llvm::outs() << p->getName() << " (level " << p->getPointerLevel() << ") points to: ";
        for(AbstractTarget t : p->getTargets()) {
            llvm::outs() << t.toString() << " ";
        }
        llvm::outs() << "\n";
    }

    llvm::outs() << "\n";
}

void Configuration::printVarInfo() {

    llvm::outs() << "\n" << vars.size() << " variables:\n";
    for(std::pair<std::string, VarRef*> v : vars) {
        llvm::outs() << v.first << " (size: " << v.second->getSize() << ")" << "\n";
    }
    llvm::outs() << "\n";
}

void Configuration::printCompositeInfo() {

    llvm::outs() << "\n" << composites.size() << " composites:\n";
    for(std::pair<std::string, CompositeRef*> c : composites) {
        llvm::outs() << c.first << " (" << c.second->getMemberCount() << " members)" << "\n";
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
    printCompositeInfo();

}

void Configuration::printPointerInfoVerbose() {

    auto allPointers = getAllPointers();


    llvm::outs() << "\n" << allPointers.size() << " Pointers:\n";
    for(auto p : allPointers) {
        llvm::outs() << "\n" << p->getName() << " (level " << p->getPointerLevel() << ") points to: ";

        for(AbstractTarget t : p->getTargets()) {
            llvm::outs() << t.toString() << " ";
        }

        llvm::outs() << "\n" << p->getName() << " was influenced by the following instructions: \n";

        for (llvm::Instruction* assocInst: p->getAssocInsts()) {
            llvm::outs() << *assocInst << "\n";
        }

        llvm::outs() << "\n\n";
    }

}


std::set<AbstractPointer *> Configuration::getAllMemberPointers() {

    std::set<AbstractPointer *> result;

    for (auto compPair : composites) {
        result.merge(compPair.second->getAllPointerMembersRecursively());
    }

    return result;

}

std::set<AbstractPointer *> Configuration::getAllPointers() {

    std::set<AbstractPointer*> allPointers = getAllMemberPointers();
    for (auto pointerEntry: pointers) {
        allPointers.insert(pointerEntry.second);
    }

    return allPointers;

}

