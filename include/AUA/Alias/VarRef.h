//
// Created by mlaupichler on 16.05.19.
//

#ifndef AUA_VARREF_H
#define AUA_VARREF_H

#include <string>


class VarRef {

private:
    std::string name;
    int alignment;

public:
    VarRef(int alignment, std::string name);

    std::string getName() {return name;};
    bool operator <(const VarRef & other) const {return name < other.name;};
};


#endif //AUA_VARREF_H
