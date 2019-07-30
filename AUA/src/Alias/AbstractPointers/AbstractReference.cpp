//
// Created by mlaupichler on 15.06.19.
//

#include <sstream>
#include <vector>
#include "AUA/Alias/AbstractPointers/AbstractReference.h"

const std::string ReferenceFlags::to_string() const {

    std::vector<std::string> flagStrings;
    flagStrings.reserve(3);

    if (global) flagStrings.emplace_back("global");
    if (member) flagStrings.emplace_back("member");
    if (param) flagStrings.emplace_back("param");

    std::ostringstream oss;
    oss << "(";

    for (int i = 0; i < flagStrings.size(); ++i) {

        oss << flagStrings[i];
        if (i != flagStrings.size() - 1)
            oss << ", ";

    }

    oss << ")";

    return oss.str();

}
