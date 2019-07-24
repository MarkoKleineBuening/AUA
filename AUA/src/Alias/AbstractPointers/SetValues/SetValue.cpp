//
// Created by mlaupichler on 15.07.19.
//

#include <AUA/Alias/AbstractPointers/SetValues/SetValueFormatException.h>
#include <AUA/Alias/AbstractPointers/SetValues/SetValue.h>

template<class T>
bool SetValue<T>::include(T *el) {

    if (!formatCheck(el)) throw SetValueFormatException();

    if (contents.find(el->getName()) != contents.end()) return false;

    contents[el->getName()] = el;
    return true;
}

template<class T>
bool SetValue<T>::exclude(T *el) {

    if (!formatCheck(el)) throw SetValueFormatException();

    if (contents.find(el->getName()) == contents.end()) return false;

    contents.erase(el->getName());
    return true;

}

template<class T>
int SetValue<T>::size() {
    return this->contents.size();
}

template<class T>
std::set<T *> SetValue<T>::asSet() {

    std::set<T *> result;

    for (auto elPair : contents) {
        result.insert(elPair.second);
    }

    return result;

}
