#include <utility>

#include <utility>

//
// Created by mlaupichler on 02.08.19.
//

#ifndef AUA_GLOBALVALUEFACTORY_H
#define AUA_GLOBALVALUEFACTORY_H


#include <llvm/IR/Module.h>
#include <sstream>
#include "AbstractPointer.h"
#include "AbstractComposite.h"

class GlobalValueFactory {

private:

    const llvm::Module* module;
    const llvm::DataLayout* dl;
    const ReferenceFlags globalFlags = ReferenceFlags(true, false, false);


public:

    explicit GlobalValueFactory(llvm::Module *module);

    AbstractPointer* buildGlobalAbstractPointer(const std::string& name);
    AbstractComposite* buildGlobalAbstractComposite(const std::string& name);
    AbstractVar* buildGlobalAbstractVar(const std::string& name);

    bool globalPointerExists(const std::string& name);
    bool globalCompositeExists(const std::string& name);
    bool globalVarExists(const std::string& name);

};

struct GlobalVariableTypeMatchException : public std::exception {

private:

    const std::string name;
    const std::string expected;

public:

    GlobalVariableTypeMatchException(std::string name, std::string expected) : name(std::move(name)),
                                                                                             expected(std::move(expected)) {}

    const char *what() const throw() {
        std::ostringstream oss;
        oss << "The global variable \" " << name << " \" does not have the desired type" << expected << ".\n";
        auto msg = oss.str();
        char* chars = (char*) std::malloc((msg.length() + 1) * sizeof(char));
        std::strcpy(chars, msg.c_str());
        return chars;
    }
};

struct GlobalVariableNotExistingException : public std::exception {

private:

    const std::string name;

public:

    GlobalVariableNotExistingException(std::string name) : name(std::move(name)) {}

    const char *what() const throw() {
        std::ostringstream oss;
        oss << "No global variable by the name of \" " << name << " \" exists.\n";
        auto msg = oss.str();
        char* chars = (char*) std::malloc((msg.length() + 1) * sizeof(char));
        std::strcpy(chars, msg.c_str());
        return chars;
    }
};




#endif //AUA_GLOBALVALUEFACTORY_H
