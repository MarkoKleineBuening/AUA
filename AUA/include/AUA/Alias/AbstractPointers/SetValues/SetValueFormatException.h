//
// Created by mlaupichler on 12.07.19.
//

#ifndef AUA_SETVALUEFORMATEXCEPTION_H
#define AUA_SETVALUEFORMATEXCEPTION_H

#include <exception>

struct SetValueFormatException : public std::exception {
    const char *what() const throw() {
        return "The given element does not fit the format of the SetValue.";
    }
};

#endif //AUA_SETVALUEFORMATEXCEPTION_H
