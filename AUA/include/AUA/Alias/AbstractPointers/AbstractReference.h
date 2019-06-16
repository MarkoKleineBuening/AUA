//
// Created by mlaupichler on 15.06.19.
//

#ifndef AUA_ABSTRACTREFERENCE_H
#define AUA_ABSTRACTREFERENCE_H


#include <string>

class AbstractReference {

protected:

    const std::string name;
    const int alignment;

    AbstractReference(std::string n, int a): name(n), alignment(a) {};

public:

    const std::string getName() const {return name;};
    const int getAlignment() const {return alignment;};

    /**
     * Returns level of reference in pointer hierarchy starting at 0 for variables.
     * @return the level.
     */
    virtual const int getLevel() = 0;


    bool operator <(const AbstractReference & other) const {return this->name < other.name;};


};


#endif //AUA_ABSTRACTREFERENCE_H
