//
// Created by mlaupichler on 27.05.19.
//

#ifndef AUA_COPYOP_H
#define AUA_COPYOP_H


#include <AUA/Alias/Configuration.h>
#include "SinglePredAndSuccOp.h"

class CopyOp : public SinglePredAndSuccOp {

private:
    std::string fromName;
    std::string toName;


protected:

    Configuration* apply(Configuration* in) override;

public:

    CopyOp(std::string fromName, std::string toName);

};


#endif //AUA_COPYOP_H
