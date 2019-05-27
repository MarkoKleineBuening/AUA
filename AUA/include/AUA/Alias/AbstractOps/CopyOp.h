//
// Created by mlaupichler on 27.05.19.
//

#ifndef AUA_COPYOP_H
#define AUA_COPYOP_H


#include <AUA/Alias/Configuration.h>
#include "PointerOperation.h"

class CopyOp : public PointerOperation{

private:
    std::string fromName;
    std::string toName;

public:

    CopyOp(std::string fromName, std::string toName);

    Configuration execute(Configuration in);

};


#endif //AUA_COPYOP_H
