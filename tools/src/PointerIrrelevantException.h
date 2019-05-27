//
// Created by mlaupichler on 27.05.19.
//

#ifndef AUA_POINTERIRRELEVANTEXCEPTION_H
#define AUA_POINTERIRRELEVANTEXCEPTION_H

#include <exception>

struct PointerIrrelevantException : public std::exception {
    const char * what () const throw() {
        return "The Instruction is irrelevant for pointers.";
    }
};

#endif //AUA_POINTERIRRELEVANTEXCEPTION_H
