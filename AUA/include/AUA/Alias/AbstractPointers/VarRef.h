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
    int size;

public:
    VarRef(std::string name, int alignment, int size);

    std::string getName() {return name;};
    int getSize() {return size;};
    bool operator <(const VarRef & other) const {return name < other.name;};
};


#endif //AUA_VARREF_H
