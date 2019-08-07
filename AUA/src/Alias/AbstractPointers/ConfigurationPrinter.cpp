//
// Created by mlaupichler on 28.07.19.
//

#include "AUA/Alias/AbstractPointers/ConfigurationPrinter.h"

std::list<Alias> ConfigurationPrinter::calculateAliases(Configuration* configuration) {

    std::list<Alias> result;

    std::set<AbstractPointer *> allPointers = getAllPointers(configuration);

    for (auto primaryPointer : allPointers) {

        for (auto otherPointer : allPointers) {

            if (primaryPointer == otherPointer ||
                primaryPointer->getPointerLevel() != otherPointer->getPointerLevel()) {
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

std::set<AbstractComposite *> ConfigurationPrinter::getAllComposites(Configuration *configuration) {

    auto result = std::set<AbstractComposite *> ();

    for (const auto& compPair : configuration->composites) {

        result.insert(compPair.second);
    }


    auto globalComposites = Configuration::global->getAllGlobalComposites();
    for (auto comp : globalComposites) {

        result.insert(comp);
    }

    return result;

}


std::set<AbstractVar *> ConfigurationPrinter::getAllVars(Configuration *configuration) {

    std::set<AbstractVar *> allVars;

    for (const auto& varPair: configuration->vars) {

        allVars.insert(varPair.second);
    }

    allVars.merge(Configuration::global->getAllGlobalVars());

    return allVars;

}


std::set<AbstractPointer *> ConfigurationPrinter::getAllMemberPointers(Configuration *configuration) {

    std::set<AbstractPointer *> result;

    auto allComposites = getAllComposites(configuration);

    for (auto composite : allComposites) {

        result.merge(composite->getAllPointerMembersRecursively());
    }

    return result;

}

std::set<AbstractPointer *> ConfigurationPrinter::getAllPointers(Configuration* configuration) {

    std::set<AbstractPointer *> allPointers = getAllMemberPointers(configuration);

    allPointers.merge(Configuration::global->getAllGlobalPointers());

    for (const auto& pointerEntry: configuration->pointers) {
        allPointers.insert(pointerEntry.second);
    }

    return allPointers;

}

void ConfigurationPrinter::printFullInfo(Configuration* configuration) {

    printPointerInfo(configuration);
    printVarInfo(configuration);
    printCompositeInfo(configuration);
}

void ConfigurationPrinter::printPointerInfo(Configuration* configuration) {

    std::set<AbstractPointer *> allPointers = getAllPointers(configuration);

    llvm::outs() << "\n" << allPointers.size() << " pointers:\n";
    for (auto p : allPointers) {
        llvm::outs() << p->to_string() << " points to: ";
        for (AbstractTarget t : p->getTargets()) {
            llvm::outs() << t.toString() << " ";
        }
        llvm::outs() << "\n";
    }

    llvm::outs() << "\n";
}

void ConfigurationPrinter::printVarInfo(Configuration* configuration) {

    auto allVars = getAllVars(configuration);

    llvm::outs() << "\n" << allVars.size() << " variables:\n";
    for (auto v : allVars) {
        llvm::outs() << v->to_string() << "\n";
    }
    llvm::outs() << "\n";
}

void ConfigurationPrinter::printCompositeInfo(Configuration* configuration) {

    auto allComposites = getAllComposites(configuration);

    llvm::outs() << "\n" << allComposites.size() << " composites:\n";
    for (auto c : allComposites) {
        llvm::outs() << c->to_string() << "\n";
    }
    llvm::outs() << "\n";

}

void ConfigurationPrinter::printAliasInfo(Configuration* configuration) {

    auto aliases = calculateAliases(configuration);

    if (aliases.empty()) {
        llvm::outs() << "\nNo aliases.\n";
        return;
    }

    llvm::outs() << "\nThe potential aliases are: \n";
    for (auto alias : aliases) {

        llvm::outs() << "\n" << alias.to_String();

    }
    llvm::outs() << "\n";

}

void ConfigurationPrinter::printFullInfoVerbose(Configuration* configuration) {


    llvm::outs() << "========================================================\n\n";

    printAliasInfo(configuration);
    printPointerInfoVerbose(configuration);
    printVarInfo(configuration);
    printCompositeInfo(configuration);

    llvm::outs() << "\n========================================================\n";

}

void ConfigurationPrinter::printPointerInfoVerbose(Configuration* configuration) {

    auto allPointers = getAllPointers(configuration);


    llvm::outs() << "\n" << allPointers.size() << " Pointers:\n";
    for (auto p : allPointers) {
        llvm::outs() << "\n" << p->to_string() << " points to: ";

        for (AbstractTarget t : p->getTargets()) {
            llvm::outs() << t.toString() << " ";
        }

        llvm::outs() << "\n" << p->getName() << " was influenced by the following instructions: \n";

        for (const llvm::Instruction *assocInst : p->getAssociatedInsts()) {
            llvm::outs() << *assocInst << "\n";
        }

        llvm::outs() << "\n\n";
    }

}
