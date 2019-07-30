//
// Created by mlaupichler on 12.07.19.
//

#ifndef AUA_POINTERSETVALUE_H
#define AUA_POINTERSETVALUE_H

#include <map>
#include <AUA/Alias/AbstractPointers/AbstractPointer.h>

class PointerSetValue {

private:

    std::map<std::string, AbstractPointer *> contents;
    const PointerFormat format;

    /**
     * Checks if the given pointer fits the format of this PointerSetValue.
     * @param ptr the pointer to check the format on.
     * @return true iff ptr fits the format.
     */
    bool formatCheck(AbstractPointer *ptr);

public:

    /**
     * Constructs PointerSetValue for pointers at the given level.
     * @param format the level of pointers in this PointerSetValue
     */
    explicit PointerSetValue(PointerFormat format);

    /**
     * Constructs PointerSetValue for pointers at the given level.
     * @param format the level of pointers in this PointerSetValue
     * @param initial set of pointers to include in this PointerSetValue initially.
     */
    PointerSetValue(PointerFormat format, std::set<AbstractPointer *> initial);

    /**
     * Includes given element pointer in this SetValue as long as no element of name el->getName() exists
     * in the SetValue already. If an element of that name exists already, the SetValue
     * remains unchanged and false is returned.
     * @param el the pointer to an element to include in the SetValue.
     * @throw SetValueFormatException if the given element does not fit the format of this SetValue as determined by <formatCheck(T* element)>.
     * @return true if the contents of this SetValue have been changed, false else.
     */
    bool include(AbstractPointer *el);

    /**
     * Excludes given element pointer from this SetValue as long as an element of name el->getName() exists
     * in the SetValue. If no element of that name exists, the SetValue
     * remains unchanged and false is returned.
     * @param el the pointer to an element to exclude from the SetValue.
     * @throw SetValueFormatException if the given element does not fit the format of this SetValue as determined by <formatCheck(T* element)>.
     * @return true if the contents of this SetValue have been changed, false else.
     */
    bool exclude(AbstractPointer *el);

    /**
     * Returns the number of elements currently included in the SetValue.
     * @return the number of elements.
     */
    int size();

    /**
     * Merges all direct targets of the pointers in the PointerSetValue and returns all targets.
     * @return all targets of the pointers in this setValue combined.
     */
    std::set<AbstractTarget> getMergedTargets();

    /**
     * Merges all indirect targets at level (ptrLevel-derefDepth-1) of the pointers in the PointerSetValue and returns all targets.
     * @return all indirect targets at the specified level of the pointers in this setValue combined.
     */
    std::set<AbstractTarget> getMergedTargets(int derefDepth);

    /**
     * Returns a list comprising all associated instructions of pointers in this PointerSetValue merged.
     * The pointers are not regarded in any particular order.
     * @return the merged associated instructions.
     */
    std::list<const llvm::Instruction*> getMergedAssociatedInsts();

    /**
     * Creates new AbstractPointer instance with the same format as this PointerSetValue and with the given name.
     * The new instance will hold all targets from all pointers in this PointerSetValue merged together.
     * @param name the name to give the new AbstractPointer instance.
     * @return the new AbstractPointer instance.
     */
    AbstractPointer *mergeToNewPointer(std::string name, ReferenceFlags flags);

    /**
     * Returns this PointerSetValue instance as a set of pointers to AbstractPointers.
     * @return a set of pointers to AbstractPointer objects included in this PointerSetValue.
     */
    std::set<AbstractPointer *> asSet();

    const PointerFormat &getFormat() const;


};


#endif //AUA_POINTERSETVALUE_H
