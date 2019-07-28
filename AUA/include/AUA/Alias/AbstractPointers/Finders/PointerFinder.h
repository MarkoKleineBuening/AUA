//
// Created by mlaupichler on 22.06.19.
//

#ifndef AUA_POINTERFINDER_H
#define AUA_POINTERFINDER_H


#include <AUA/Alias/AbstractPointers/SetValues/PointerSetValue.h>
#include <sstream>
#include "AUA/Alias/AbstractPointers/Configuration.h"

class PointerFinder {

protected:

    const PointerFormat expectedFormat;

public:

    explicit PointerFinder(const PointerFormat &expectedFormat);

    virtual PointerSetValue *findPointers(Configuration *configuration) const = 0;

    virtual std::list<const llvm::Instruction *> getAssociatedInsts() const = 0;

};




struct PointerFinderFormatException : public std::exception {

private:

    PointerFormat expectedFormat;
    PointerFormat actualFormat;

public:

    PointerFinderFormatException(PointerFormat expectedFormat, PointerFormat actualFormat)
            : expectedFormat(expectedFormat), actualFormat(actualFormat) {}

    const char *what() const throw() {

        std::ostringstream oss;
        oss << "The actual format  (level = " << actualFormat.level << ") did not fit the expected format (level = " << expectedFormat.level << ")!\n";
        auto msg = oss.str();
        char* chars = (char*) std::malloc((msg.length() + 1) * sizeof(char));
        std::strcpy(chars, msg.c_str());
        return chars;
    }
};


#endif //AUA_POINTERFINDER_H
