//
// Created by mlaupichler on 16.05.19.
//

#ifndef AUA_ABSTRACTVAR_H
#define AUA_ABSTRACTVAR_H

#include <string>
#include "AbstractReference.h"


class AbstractVar : public AbstractReference {

private:

    const int size;

public:

    AbstractVar(std::string n, int s);

    AbstractVar(std::string n, int s, ReferenceFlags flags);


    const int getPointerLevel() override { return 0; };

    const std::string to_string() override;


    const int getSize() { return size; };
};


#endif //AUA_ABSTRACTVAR_H
