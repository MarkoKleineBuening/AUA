//
// Created by mlaupichler on 04.08.19.
//

#ifndef AUA_DUMMYLOOPLATCHENDOP_H
#define AUA_DUMMYLOOPLATCHENDOP_H

#include "PointerOperation.h"
#include "SinglePredAndSuccOp.h"

/**
 * PointerOperation to act as a dummy final operation for LOOP LATCH blocks. Only passes on call to next operation when called.
 */
class DummyLoopLatchEndOp : public SinglePredAndSuccOp {

protected:
    void consume(Configuration *result) override;

    Configuration *apply(Configuration *in) override;

};


#endif //AUA_DUMMYLOOPLATCHENDOP_H
