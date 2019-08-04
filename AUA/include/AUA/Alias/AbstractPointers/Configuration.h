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

    static void setGlobalConfiguration(GlobalConfiguration* global);


};


struct ConfigurationUnknownTargetNameException : public std::exception {
    const char *what() const throw() {
        return "The given name for the desired target was not known in the configuration.";
    }
};

struct GlobalConfigurationAlreadySetException : public std::exception {
    const char *what() const throw() {
        return "The global configuration has already been set once.";
    }
};

#endif //AUA_CONFIGURATION_H
