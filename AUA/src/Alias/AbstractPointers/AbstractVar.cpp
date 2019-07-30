//
// Created by mlaupichler on 16.05.19.
//


#include <AUA/Alias/AbstractPointers/AbstractVar.h>
#include <sstream>

AbstractVar::AbstractVar(std::string n, int s) : AbstractReference(n), size(s) {}

AbstractVar::AbstractVar(std::string n, int s, ReferenceFlags flags) : AbstractReference(n, flags), size(s) {

}

const std::string AbstractVar::to_string() {

    std::ostringstream oss;
    oss << name << " (size: " << size << ", flags: " << flags.to_string() << ")";

    return oss.str();

}
