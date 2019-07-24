//
// Created by mlaupichler on 12.07.19.
//

#ifndef AUA_COMPOSITESETVALUE_H
#define AUA_COMPOSITESETVALUE_H


#include <map>
#include <AUA/Alias/AbstractPointers/AbstractComposite.h>

class CompositeSetValue {

private:

    std::map<std::string, AbstractComposite *> contents;

    const CompositeFormat format;


    bool formatCheck(AbstractComposite *composite);


public:

    explicit CompositeSetValue(CompositeFormat format);

    CompositeSetValue(CompositeFormat format, std::set<AbstractComposite *> initial);

    /**
     * Includes given element pointer in this SetValue as long as no element of name el->getName() exists
     * in the SetValue already. If an element of that name exists already, the SetValue
     * remains unchanged and false is returned.
     * @param el the pointer to an element to include in the SetValue.
     * @throw SetValueFormatException if the given element does not fit the format of this SetValue as determined by <formatCheck(T* element)>.
     * @return true if the contents of this SetValue have been changed, false else.
     */
    bool include(AbstractComposite *el);

    /**
     * Excludes given element pointer from this SetValue as long as an element of name el->getName() exists
     * in the SetValue. If no element of that name exists, the SetValue
     * remains unchanged and false is returned.
     * @param el the pointer to an element to exclude from the SetValue.
     * @throw SetValueFormatException if the given element does not fit the format of this SetValue as determined by <formatCheck(T* element)>.
     * @return true if the contents of this SetValue have been changed, false else.
     */
    bool exclude(AbstractComposite *el);

    /**
     * Returns the number of elements currently included in the SetValue.
     * @return the number of elements.
     */
    int size();

    /**
     * Creates a new CompositeRef instance with the given name that has the same format as
     * the CompositeRef objects in this CompositeSetValue. The new CompositeRefs pointers
     * and the pointers of its sub composites are merged from all CompositeRefs in this CompositeSetValue.
     * @param name the name to give the new CompositeRef.
     * @return the merged CompositeRef.
     */
    AbstractComposite *mergeToNewComposite(std::string name);

    /**
     * Returns this CompositeSetValue instance as a set of pointers to CompositeRefs.
     * @return a set of pointers to AbstractComposite objects included in this CompositeSetValue.
     */
    std::set<AbstractComposite *> asSet();

    const CompositeFormat &getFormat() const;

};


#endif //AUA_COMPOSITESETVALUE_H
