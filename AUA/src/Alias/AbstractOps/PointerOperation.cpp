//
// Created by mlaupichler on 16.05.19.
//

#include <AUA/Alias/AbstractOps/PointerOperation.h>
#include <llvm/Support/raw_ostream.h>
#include <AUA/Alias/AbstractPointers/ConfigurationPrinter.h>

// PRIVATE

void PointerOperation::consume(Configuration *result) {

    ConfigurationPrinter::printFullInfo(result);

    // TODO add consumer that extracts aliases from Configuration. Best in parallel.
}

