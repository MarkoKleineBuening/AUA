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
    
public:

    static GlobalConfiguration *global;

    std::map<std::string, AbstractPointer *> pointers;
    std::map<std::string, AbstractVar *> vars;
    std::map<std::string, AbstractComposite *> composites;

    explicit Configuration();

    void merge(Configuration *other);

    Configuration *getDeepCopy();


};


struct ConfigurationUnknownTargetNameException : public std::exception {
    const char *what() const throw() {
        return "The given name for the desired target was not known in the configuration.";
    }
};


#endif //AUA_CONFIGURATION_H
