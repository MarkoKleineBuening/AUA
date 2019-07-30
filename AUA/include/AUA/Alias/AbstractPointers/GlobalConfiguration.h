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


class GlobalConfiguration {

private:

    std::map<std::string, AbstractFunction *> globalFunctions;
    std::map<std::string, AbstractPointer *> globalPointers;
    std::map<std::string, AbstractComposite *> globalComposites;
    std::map<std::string, AbstractVar *> globalVars;

public:

    bool hasGlobalFunction(std::string name);
    bool hasGlobalPointer(std::string name);
    bool hasGlobalComposite(std::string name);
    bool hasGlobalVar(std::string name);

    void addGlobalFunction(std::string name, AbstractFunction *function);
    void addGlobalPointer(std::string name, AbstractPointer *pointer);
    void addGlobalComposite(std::string name, AbstractComposite *composite);
    void addGlobalVar(std::string name, AbstractVar *var);

    AbstractFunction *getGlobalFunction(std::string name);
    AbstractPointer *getGlobalPointer(std::string name);
    AbstractComposite *getGlobalComposite(std::string name);
    AbstractVar *getGlobalVar(std::string name);

    std::set<AbstractPointer*> getAllGlobalPointers();
    std::set<AbstractComposite*> getAllGlobalComposites();

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
