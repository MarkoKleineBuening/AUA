//
// Created by mlaupichler on 18.07.19.
//

#ifndef AUA_GLOBALCONFIGURATION_H
#define AUA_GLOBALCONFIGURATION_H


#include <string>
#include <map>
#include "AbstractFunction.h"
#include "AbstractPointer.h"
#include "AbstractComposite.h"
#include "AbstractVar.h"
#include "GlobalValueFactory.h"


class GlobalConfiguration {

private:

    GlobalValueFactory* globalValueFactory;

    std::map<std::string, AbstractFunction *> globalFunctions;
    std::map<std::string, AbstractPointer *> globalPointers;
    std::map<std::string, AbstractComposite *> globalComposites;
    std::map<std::string, AbstractVar *> globalVars;

public:

    GlobalConfiguration(GlobalValueFactory *globalValueFactory);

    AbstractReference *getGlobalReference(const std::string& name);

    AbstractPointer *getGlobalPointer(const std::string& name);
    AbstractComposite *getGlobalComposite(const std::string& name);
    AbstractVar *getGlobalVar(const std::string& name);

    std::set<AbstractPointer*> getAllGlobalPointers();
    std::set<AbstractComposite*> getAllGlobalComposites();

    bool hasGlobalFunction(const std::string& name);
    AbstractFunction *getGlobalFunction(const std::string& name);
    void addGlobalFunction(AbstractFunction* function);

};

struct UnknownGlobalNameException : public std::exception {
    const char *what() const throw() {
        return "The given name is not associated with any global value of the expected type.";
    }
};

struct GlobalNameAlreadyKnownException : public std::exception {
    const char *what() const throw() {
        return "The given name is already associated with a global variable.";
    }
};


#endif //AUA_GLOBALCONFIGURATION_H
