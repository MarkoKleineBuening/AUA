//
// Created by mlaupichler on 12.05.19.
//

#ifndef AUA_POINTER_H
#define AUA_POINTER_H

#include <string>
#include <set>
#include "VarRef.h"

class Pointer {

private:

    uint64_t alignment;
    std::string name;
    std::set<VarRef*> varRefs;

public:

    Pointer(int alignment, std::string name);

    uint64_t getAlignment() {return alignment;};
    std::string getName() {return name;};
    std::set<VarRef*> getVarRefs(){return varRefs;};

    bool equals(Pointer* other) {return name == other->getName();};
    bool operator <(const Pointer & other) const {return name < other.name;};

    void onlyPointTo(VarRef* var);
    void alsoPointTo(VarRef* var);
    void copyPointersFrom(Pointer* other);


};




#endif //AUA_POINTER_H
