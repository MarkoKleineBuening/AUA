//
// Created by mlaupichler on 15.07.19.
//

#ifndef AUA_SETVALUE_H
#define AUA_SETVALUE_H

#include <set>
#include <map>
#include <string>

/**
 * Wrapper class for a set of pointers to AbstractReference objects of the same format.
 * Uniqueness in the set is given by name of the AbstractReference objects.
 * @tparam T the type of AbstractReference.
 */
template<class T>
class SetValue {

protected:

    std::map<std::string, T *> contents;

    /**
     * Checks if the format of the given element fits the one of this SetValue.
     * @param element the element to check the format against.
     * @return true if the format fits, false else.
     */
    virtual bool formatCheck(T *element) = 0;

public:

    /**
     * Includes given element pointer in this SetValue as long as no element of name el->getName() exists
     * in the SetValue already. If an element of that name exists already, the SetValue
     * remains unchanged and false is returned.
     * @param el the pointer to an element to include in the SetValue.
     * @throw SetValueFormatException if the given element does not fit the format of this SetValue as determined by <formatCheck(T* element)>.
     * @return true if the contents of this SetValue have been changed, false else.
     */
    bool include(T *el);

    /**
     * Excludes given element pointer from this SetValue as long as an element of name el->getName() exists
     * in the SetValue. If no element of that name exists, the SetValue
     * remains unchanged and false is returned.
     * @param el the pointer to an element to exclude from the SetValue.
     * @throw SetValueFormatException if the given element does not fit the format of this SetValue as determined by <formatCheck(T* element)>.
     * @return true if the contents of this SetValue have been changed, false else.
     */
    bool exclude(T *el);

    /**
     * Returns the number of elements currently included in the SetValue.
     * @return the number of elements.
     */
    int size();

    /**
     * Returns this SetValue as a set of all included element pointers.
     * @return the included element pointers as a set.
     */
    std::set<T *> asSet();

};


#endif //AUA_SETVALUE_H
