//
// Created by mlaupichler on 16.05.19.
//

#ifndef AUA_VARREF_H
#define AUA_VARREF_H

#include <string>
#include "AbstractReference.h"


class VarRef : public AbstractReference{

private:

    const int size;

public:

    VarRef(std::string n, int a, int s);


    const int getLevel() override {return 0;};


    const int getSize() {return size;};
};


#endif //AUA_VARREF_H
