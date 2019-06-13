//
// Created by mlaupichler on 12.05.19.
//

#ifndef AUA_ABSTRACTPOINTER_H
#define AUA_ABSTRACTPOINTER_H

#include <string>
#include <set>
#include "VarRef.h"
#include "AbstractTarget.h"

class AbstractPointer {

private:

    int alignment;
    std::string name;
    std::set<AbstractTarget> targets;

public:

    AbstractPointer(int alignment, std::string name);

    int getAlignment() {return alignment;};
    std::string getName() {return name;};
    std::set<AbstractTarget> getTargets(){return targets;};

    AbstractPointer* getCopy();

    bool equals(AbstractPointer* other) {return name == other->getName();};
    bool operator <(const AbstractPointer & other) const {return name < other.name;};

    void onlyPointTo(AbstractTarget var);
    void alsoPointTo(AbstractTarget var);
    void copyTargetsFrom(AbstractPointer *other);
    void mergeTargets(AbstractPointer* other);


};




#endif //AUA_ABSTRACTPOINTER_H
