//
// Created by mlaupichler on 27.05.19.
//

#ifndef AUA_CONFIGURATION_H
#define AUA_CONFIGURATION_H


#include <string>
#include <map>
#include <AUA/Alias/AbstractPointers/AbstractPointer.h>

class Configuration {

private:

public:

    std::map<std::string, AbstractPointer*> pointers;
    std::map<std::string, VarRef*> vars;
    void merge(Configuration* other);
    Configuration* getDeepCopy();

    void printFullInfo();
    void printPointerInfo();
    void printVarInfo();

};


#endif //AUA_CONFIGURATION_H
