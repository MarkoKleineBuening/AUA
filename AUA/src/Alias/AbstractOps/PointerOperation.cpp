//
// Created by mlaupichler on 16.05.19.
//

#include <AUA/Alias/AbstractOps/PointerOperation.h>
#include <llvm/Support/raw_ostream.h>

// PRIVATE

void PointerOperation::consume(Configuration* result) {

    result->printFullInfo();

    // TODO add consumer that extracts aliases from Configuration. Best in parallel.
}

