//
// Created by mlaupichler on 15.06.19.
//

#ifndef AUA_ABSTRACTREFERENCE_H
#define AUA_ABSTRACTREFERENCE_H


#include <string>

struct ReferenceFlags {

    /**
     * specifies whether this reference is a global value.
     */
    const bool global;

    /**
     * specifies whether this reference is a member of a composite (struct or array).
     */
    const bool member;

    /**
     * specifies whether this reference is a parameter of the function it lives in.
     */
    const bool param;

    /**
     * Default constructor.
     */
    ReferenceFlags() : global(false), member(false), param(false) {}

    /**
     * Initializer constructor.
     * @param global specifies wether this reference is a global value.
     * @param member specifies wether this reference is a member of a composite (struct or array).
     * @param param specifies wether this reference is a parameter of the function it lives in.
     */
    ReferenceFlags(const bool global, const bool member, const bool param) : global(global), member(member), param(param) {}

    const std::string to_string() const;

};


class AbstractReference {

protected:

    const std::string name;

    const ReferenceFlags flags;


    explicit AbstractReference(const std::string n) : name(n), flags(ReferenceFlags()) {};
    AbstractReference(const std::string n, const ReferenceFlags flags) : name(n), flags(flags) {};

public:

    const std::string getName() const { return name; }

    const ReferenceFlags &getFlags() const {
        return flags;
    }

    /**
     * Returns level of reference in pointer hierarchy starting at 0 for variables.
     * @return the level.
     */
    virtual const int getPointerLevel() = 0;

    virtual const std::string to_string() = 0;


    bool operator<(const AbstractReference &other) const { return this->name < other.name; };


};


#endif //AUA_ABSTRACTREFERENCE_H
