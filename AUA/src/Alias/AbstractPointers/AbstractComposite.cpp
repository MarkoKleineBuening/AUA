//
// Created by mlaupichler on 20.06.19.
//

#include <AUA/Alias/AbstractPointers/CompositeFormat.h>
#include <sstream>
#include "AUA/Alias/AbstractPointers/AbstractComposite.h"

AbstractComposite::AbstractComposite(std::string name, const CompositeFormat format) : AbstractReference(name),
                                                                                        format(format) {

    ReferenceFlags memberFlags = ReferenceFlags(false, true, false);

    // Create all sub composites immediately (pointers are created lazily)

    for (auto subCompFormatPair : format.compositeMemberFormats) {

        int idx = subCompFormatPair.first;
        CompositeFormat subCompFormat = subCompFormatPair.second;

        composites[idx] = new AbstractComposite(generateMemberName(idx), subCompFormat, memberFlags);

    }

}

AbstractComposite::AbstractComposite(std::string name, CompositeFormat format, ReferenceFlags flags) : AbstractReference(name, flags), format(format) {

    // Create all sub composites immediately (pointers are created lazily)

    ReferenceFlags memberFlags = ReferenceFlags(false, true, false);

    for (auto subCompFormatPair : format.compositeMemberFormats) {

        int idx = subCompFormatPair.first;
        CompositeFormat subCompFormat = subCompFormatPair.second;

        composites[idx] = new AbstractComposite(generateMemberName(idx), subCompFormat, memberFlags);

    }

}



AbstractComposite::AbstractComposite(const std::string name, CompositeFormat format,
                                     std::map<int, AbstractPointer *> pointers,
                                     std::map<int, AbstractComposite *> composites, ReferenceFlags flags) :
        AbstractReference(name, flags), format(format),
        pointers(pointers), composites(composites) {}

const int AbstractComposite::getPointerLevel() {
    return 0;
}

AbstractPointer *AbstractComposite::getPointerMember(int memberIdx) {

    if (memberIdx >= format.memberCount) throw MemberNotFoundException();

    if (format.pointerMemberFormats.find(memberIdx) == format.pointerMemberFormats.end()) {
        throw MemberNotFoundException();
    }

    // Generate pointer object lazily
    if (pointers.find(memberIdx) == pointers.end()) {
        auto ptrFormat = format.pointerMemberFormats.at(memberIdx);
        AbstractPointer *ptr = new AbstractPointer(generateMemberName(memberIdx), ptrFormat, ReferenceFlags(false, true, false));
        pointers[memberIdx] = ptr;
    }

    return pointers[memberIdx];

}


AbstractComposite *AbstractComposite::getCompositeMember(int memberIdx) {


    if (memberIdx >= format.memberCount) throw MemberNotFoundException();

    if (format.compositeMemberFormats.find(memberIdx) == format.compositeMemberFormats.end()) {
        throw MemberNotFoundException();
    }

    if (composites.find(memberIdx) == composites.end()) {
        throw MemberNotFoundException();
    }

    return composites[memberIdx];

}

std::string AbstractComposite::generateMemberName(int idx) {
    return getName() + "[" + std::to_string(idx) + "]";
}

AbstractTarget AbstractComposite::getMemberTarget(int memberIdx) {

    if (memberIdx >= format.memberCount) throw MemberNotFoundException();

    int size = (memberIdx < format.memberCount - 1 ? format.offsets[memberIdx + 1] : format.totalSize) -
               format.offsets[memberIdx];

    if (format.pointerMemberFormats.find(memberIdx) != format.pointerMemberFormats.end()) {

        // Member is pointer
        AbstractPointer *ptr = getPointerMember(memberIdx);

        return AbstractTarget(ptr, 0, size);
    }

    if (format.compositeMemberFormats.find(memberIdx) != format.compositeMemberFormats.end()) {

        // Member is a composite
        AbstractComposite *comp = getCompositeMember(memberIdx);

        return AbstractTarget(comp, 0, size);
    }

    // Member is a variable
    return AbstractTarget(this, format.offsets[memberIdx], size);

}

int AbstractComposite::getMemberCount() {
    return format.memberCount;
}

std::set<AbstractPointer *> AbstractComposite::getAllPointerMembers() {

    std::set<AbstractPointer *> result;

    for (auto ptrPair : pointers) {
        result.insert(ptrPair.second);
    }

    return result;


}

std::set<AbstractComposite *> AbstractComposite::getAllCompositeMembers() {

    std::set<AbstractComposite *> result;

    for (auto compPair : composites) {
        result.insert(compPair.second);
    }

    return result;
}

std::set<AbstractPointer *> AbstractComposite::getAllPointerMembersRecursively() {

    auto result = getAllPointerMembers();

    for (auto compPair : composites) {

        result.merge(compPair.second->getAllPointerMembersRecursively());

    }

    return result;

}

AbstractComposite *AbstractComposite::getSubCompositeRecursively(std::list<int> compMemberIndices) {

    AbstractComposite *comp = this;

    for (auto LI = compMemberIndices.begin(), LE = compMemberIndices.end(); LI != LE; ++LI) {

        comp = comp->getCompositeMember(*LI);

    }

    return comp;

}

void AbstractComposite::copyContentsTo(AbstractComposite *other) {

    assert(this->format == other->format);

    other->pointers = this->pointers;
    other->composites = this->composites;

}


AbstractComposite *AbstractComposite::getDeepCopy() {

    std::map<int, AbstractPointer *> pointerCopies;
    for (auto ptrPair : pointers) {
        pointerCopies[ptrPair.first] = ptrPair.second->getCopy();
    }

    std::map<int, AbstractComposite *> compositeCopies;
    for (auto compPair : composites) {
        compositeCopies[compPair.first] = compPair.second->getDeepCopy();
    }

    AbstractComposite *copy = new AbstractComposite(this->name, this->format, pointerCopies, compositeCopies, this->flags);

    return copy;


}


void AbstractComposite::merge(AbstractComposite *other) {

    assert(this->format == other->format);

    for (auto ptrPair : other->pointers) {

        if (pointers.find(ptrPair.first) == pointers.end()) {

            assert(format.pointerMemberFormats.find(ptrPair.first) != format.pointerMemberFormats.end());
            pointers[ptrPair.first] = ptrPair.second;

        } else {
            pointers[ptrPair.first]->merge(ptrPair.second);
        }


    }

    for (auto compPair : composites) {
        assert(composites.find(compPair.first) != composites.end());
        composites[compPair.first]->merge(compPair.second);
    }

}

const CompositeFormat &AbstractComposite::getFormat() const {
    return format;
}

const std::string AbstractComposite::to_string() {

    std::ostringstream oss;
    oss << name << " (member count: " << format.memberCount << ", flags: " << flags.to_string() << ")";

    return oss.str();

}

const int AbstractComposite::getSize() {
    return format.totalSize;
}
