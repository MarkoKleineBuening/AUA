//
// Created by mlaupichler on 12.05.19.
//

#ifndef AUA_REFERENCE_H
#define AUA_REFERENCE_H

#include "RefType.h"
#include <string>

class Reference {

private:

    int id;
    uint64_t size;
    RefType type;
    std::string name;

public:
    Reference(uint64_t s, RefType t, std::string n);
    uint64_t getSize() {return size;};
    RefType getType() {return type;};
    std::string getName() {return name;};
    int getID(){return id;};

};


#endif //AUA_REFERENCE_H
