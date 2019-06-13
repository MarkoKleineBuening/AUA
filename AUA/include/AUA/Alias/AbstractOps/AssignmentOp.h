//
// Created by mlaupichler on 27.05.19.
//

#ifndef AUA_ASSIGNMENTOP_H
#define AUA_ASSIGNMENTOP_H


#include <AUA/Alias/Configuration.h>
#include "SinglePredAndSuccOp.h"

class AssignmentOp : public SinglePredAndSuccOp {

private:

    std::string pointerName;
    std::string varName;


protected:

    Configuration* apply(Configuration* in) override;

public:

    AssignmentOp(std::string ptrName, std::string varName);

};


#endif //AUA_ASSIGNMENTOP_H
