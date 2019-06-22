//
// Created by mlaupichler on 27.05.19.
//

#ifndef AUA_CONFIGURATION_H
#define AUA_CONFIGURATION_H


#include <string>
#include <map>
#include <list>
#include <AUA/Alias/AbstractPointers/AbstractPointer.h>
#include "Alias.h"
#include "CompositeRef.h"


class Configuration {

private:

    std::list<Alias> calculateAliases();

    std::set<AbstractPointer *> getAllMemberPointers();
    std::set<AbstractPointer *> getAllPointers();

public:
    std::map<std::string, AbstractPointer*> pointers;
    std::map<std::string, VarRef*> vars;
    std::map<std::string, CompositeRef*> composites;
    void merge(Configuration* other);

    Configuration* getDeepCopy();
    void printFullInfo();
    void printPointerInfo();
    void printVarInfo();
    void printCompositeInfo();

    void printAliasInfo();
    void printFullInfoVerbose();

    void printPointerInfoVerbose();
};


#endif //AUA_CONFIGURATION_H
