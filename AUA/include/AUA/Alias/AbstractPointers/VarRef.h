//
// Created by mlaupichler on 16.05.19.
//

#ifndef AUA_VARREF_H
#define AUA_VARREF_H

#include <string>


class VarRef {

private:
    const std::string name;
    const int alignment;
    const int size;

public:
    VarRef(std::string n, int a, int s) : name(n), alignment(a), size(s) {}

    const std::string getName() {return name;};
    const int getSize() {return size;};
    bool operator <(const VarRef & other) const {return name < other.name;};
};


#endif //AUA_VARREF_H
