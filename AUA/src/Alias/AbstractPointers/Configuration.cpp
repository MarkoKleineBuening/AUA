//
// Created by mlaupichler on 27.05.19.
//

#include "AUA/Alias/AbstractPointers/Configuration.h"

#include <llvm/Support/FileSystem.h>

// PRIVATE

GlobalConfiguration* Configuration::global = nullptr;



// PUBLIC


/**
 * Merges this Configuration with another given Configuration.
 * Afterwards this Configuration holds all pointers, composites and variables from both Configurations
 * and all pointers have the targets from both configurations.
 * @param other the Configuration to merge with.
 */
void Configuration::merge(Configuration *other) {

    // merge pointers

    for (auto pointerEntry : other->pointers) {

        if (this->pointers.find(pointerEntry.first) == this->pointers.end()) {
            this->pointers.insert(pointerEntry);
        } else {

            std::string pointerName = pointerEntry.first;

            AbstractPointer *pointerInThis = this->pointers.at(pointerName);
            AbstractPointer *pointerInOther = pointerEntry.second;


            pointerInThis->merge(pointerInOther);

        }
    }


    // merge composites
    for (auto compositeEntry : other->composites) {

        if (this->composites.find(compositeEntry.first) == this->composites.end()) {
            this->composites.insert(compositeEntry);
        } else {

            std::string compositeName = compositeEntry.first;

            AbstractComposite *compositeInThis = this->composites.at(compositeName);
            AbstractComposite *compositeInOther = compositeEntry.second;


            compositeInThis->merge(compositeInOther);

        }
    }


    //merge vars

    this->vars.merge(other->vars);

}

Configuration *Configuration::getDeepCopy() {

    Configuration *copy = new Configuration();

    // Get actual copies of pointers
    for (auto pointerEntry : this->pointers) {
        AbstractPointer *ptrCopy = pointerEntry.second->getCopy();
        copy->pointers[pointerEntry.first] = ptrCopy;

    }

    // Get actual copies of composites
    for (auto compositeEntry : this->composites) {
        AbstractComposite *compositeCopy = compositeEntry.second->getDeepCopy();
        copy->composites[compositeEntry.first] = compositeCopy;

    }

    // Variables are constant, and as such don't need to be actually copied
    copy->vars = this->vars;

    return copy;

}

void Configuration::setGlobalConfiguration(GlobalConfiguration *global) {

    if (Configuration::global != nullptr) throw GlobalConfigurationAlreadySetException();

    Configuration::global = global;
}


Configuration::Configuration() = default;






