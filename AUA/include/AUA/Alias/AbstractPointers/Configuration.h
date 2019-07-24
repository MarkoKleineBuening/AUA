//
// Created by mlaupichler on 27.05.19.
//

#ifndef AUA_CONFIGURATION_H
#define AUA_CONFIGURATION_H


#include <string>
#include <map>
#include <list>
#include <AUA/Alias/AbstractPointers/AbstractPointer.h>
#include <AUA/Alias/AbstractPointers/AbstractComposite.h>
#include <AUA/Alias/AbstractPointers/Alias.h>

class GlobalConfiguration;


class Configuration {

private:

    std::list<Alias> calculateAliases();

    std::set<AbstractPointer *> getAllMemberPointers();

    std::set<AbstractPointer *> getAllPointers();

public:

    static GlobalConfiguration *global;

    std::map<std::string, AbstractPointer *> pointers;
    std::map<std::string, AbstractVar *> vars;
    std::map<std::string, AbstractComposite *> composites;

    explicit Configuration();



    void merge(Configuration *other);

    Configuration *getDeepCopy();

    void printFullInfo();

    void printPointerInfo();

    void printVarInfo();

    void printCompositeInfo();

    void printAliasInfo();

    void printFullInfoVerbose();

    void printPointerInfoVerbose();
};


#endif //AUA_CONFIGURATION_H
