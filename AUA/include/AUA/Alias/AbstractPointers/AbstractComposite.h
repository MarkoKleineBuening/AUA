//
// Created by mlaupichler on 20.06.19.
//

#ifndef AUA_ABSTRACTCOMPOSITE_H
#define AUA_ABSTRACTCOMPOSITE_H

#include <map>
#include <list>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/DataLayout.h>
#include "AbstractReference.h"
#include "AbstractPointer.h"
#include "CompositeFormat.h"
#include "PointerFormat.h"


class AbstractComposite : public AbstractReference {


private:

    CompositeFormat format;

    /**
     * Pointer objects are created lazily
     */
    std::map<int, AbstractPointer *> pointers;

    /**
     * Composite members are created immediately
     */
    std::map<int, AbstractComposite *> composites;


    std::string generateMemberName(int idx);

    /**
     * Constructor for copying.
     * @param name name of the composite.
     * @param memberCount number of members in the composite.
     * @param totalSize total Size of the composite.
     * @param pointers pointer members in the composite with regarding indices.
     * @param composites composite members in the composite with regarding indices.
     * @param memberMeta member meta information for the composite.
     */
    AbstractComposite(std::string name, CompositeFormat format,
                      std::map<int, AbstractPointer *> pointers,
                      std::map<int, AbstractComposite *> composites);

public:


    const CompositeFormat &getFormat() const;

    AbstractComposite(std::string name, CompositeFormat format);

    AbstractComposite(const std::string &name, llvm::CompositeType *type, llvm::DataLayout *&dl);

    /**
     * Returns an actual copy at a new place in memory with all members copied too.
     * @return the deep copy.
     */
    AbstractComposite *getDeepCopy();


    /**
     * Copies the contents of this composite, i.e. the pointer and composite members, to another given composite.
     * All other attributes of the other composite remain unchanged.
     * Needs the composites to be of the same number of members, of the same type in all members and of the same total size.
     * @param other the composite to copy to.
     */
    void copyContentsTo(AbstractComposite *other);

    const int getPointerLevel() override;


    /**
     * Returns the pointer object for the member at index memberIdx if this index corresponds to a pointer type member.
     * @param memberIdx the index of the desired pointer member.
     * @return the pointer object for the member at index memberIdx if this index corresponds to a pointer type member.
     * @throws MemberNotFoundExeption if the given index does not correspond to a pointer member.
     */
    AbstractPointer *getPointerMember(int memberIdx);


    /**
     * Returns the composite object for the member at index memberIdx if this index corresponds to a composite type member.
     * @param memberIdx the index of the desired composite member.
     * @return the composite object for the member at index memberIdx if this index corresponds to a composite type member.
     * @throws MemberNotFoundExeption if the given index does not correspond to a composite member.
     */
    AbstractComposite *getCompositeMember(int memberIdx);

    /**
     * Generates and returns a target to the member at the given index.
     * @param memberIdx the index of the member to target.
     * @return an AbstractTarget object targeting the member at memberIdx.
     */
    AbstractTarget getMemberTarget(int memberIdx);

    int getMemberCount();

    std::set<AbstractPointer *> getAllPointerMembers();


    std::set<AbstractComposite *> getAllCompositeMembers();

    AbstractComposite *getSubCompositeRecursively(std::list<int> compMemberIndices);

    std::set<AbstractPointer *> getAllPointerMembersRecursively();

    /**
     * Merges all pointers of the given Composite and of all its sub composites into this composite and its sub composites. The given composite remains unchanged.
     * @param other the given Composite to take pointers for merging from. Needs o have same format as this composite.
     */
    void merge(AbstractComposite *other);


};

class MemberNotFoundException : public std::exception {

};


#endif //AUA_ABSTRACTCOMPOSITE_H
