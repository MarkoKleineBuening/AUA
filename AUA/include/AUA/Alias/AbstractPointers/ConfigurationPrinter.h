//
// Created by mlaupichler on 28.07.19.
//

#ifndef AUA_CONFIGURATIONPRINTER_H
#define AUA_CONFIGURATIONPRINTER_H


#include "Alias.h"
#include "Configuration.h"
#include "GlobalConfiguration.h"

class ConfigurationPrinter {

private:

    static std::list<Alias> calculateAliases(Configuration* configuration);

    static std::set<AbstractComposite *> getAllComposites(Configuration* configuration);

    static std::set<AbstractPointer *> getAllMemberPointers(Configuration *composites);

    static std::set<AbstractPointer *> getAllPointers(Configuration* configuration);


public:

    static void printFullInfo(Configuration* configuration);

    static void printPointerInfo(Configuration* configuration);

    static void printVarInfo(Configuration* configuration);

    static void printCompositeInfo(Configuration* configuration);

    static void printAliasInfo(Configuration* configuration);

    static void printFullInfoVerbose(Configuration* configuration);

    static void printPointerInfoVerbose(Configuration* configuration);

};


#endif //AUA_CONFIGURATIONPRINTER_H
