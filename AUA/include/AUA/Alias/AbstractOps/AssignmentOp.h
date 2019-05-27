//
// Created by mlaupichler on 27.05.19.
//

#ifndef AUA_ASSIGNMENTOP_H
#define AUA_ASSIGNMENTOP_H


#include <AUA/Alias/Configuration.h>
#include "PointerOperation.h"

class AssignmentOp : public PointerOperation {

private:

    std::string pointerName;
    std::string varName;

public:

    AssignmentOp(std::string ptrName, std::string varName);

    Configuration execute(Configuration in);

};


#endif //AUA_ASSIGNMENTOP_H
