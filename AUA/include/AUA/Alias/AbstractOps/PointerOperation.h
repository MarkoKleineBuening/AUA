//
// Created by mlaupichler on 16.05.19.
//

#ifndef AUA_POINTEROPERATION_H
#define AUA_POINTEROPERATION_H

#include <map>
#include <AUA/Alias/AbstractPointers/AbstractPointer.h>

/**
 * Abstract class for abstracting llvm instructions to operations that influence pointers.
 */
class PointerOperation {

protected:

    std::set<PointerOperation*> previous;
    std::set<PointerOperation*> following;

public:

    /**
     * Executes the operation on a given configuration and returns the resulting configuration.
     * @param in the configuration to work on.
     * @return the resulting configuration.
     */
    virtual Configuration execute(Configuration in) = 0;

};


#endif //AUA_POINTEROPERATION_H
