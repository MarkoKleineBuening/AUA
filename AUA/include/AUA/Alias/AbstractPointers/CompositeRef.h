//
// Created by mlaupichler on 20.06.19.
//

#ifndef AUA_COMPOSITEREF_H
#define AUA_COMPOSITEREF_H

#include <map>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/DataLayout.h>
#include "AbstractReference.h"
#include "AbstractPointer.h"

enum MemberType  {VAR, PTR, COMP};

struct MemberMetaInfo {

    const int size;
    const int byteOffset;
    const int pointerLevel;
    const MemberType type;

    MemberMetaInfo(const int size, const int byteOffset, const int pointerLevel, const MemberType type);

};


class CompositeRef : public AbstractReference{


private:

    int memberCount;
    int totalSize;

    /**
     * Pointer objects are created lazily
     */
    std::map<int, AbstractPointer*> pointers;

    /**
     * Composite members are created immediately
     */
    std::map<int, CompositeRef*> composites;

    std::vector<MemberMetaInfo*> memberMeta;


    MemberMetaInfo *getMetaForType(llvm::Type *type, int offset, llvm::DataLayout *dl);

    std::string generateMemberName(int idx);

public:



    CompositeRef(const std::string name, int alignment, llvm::CompositeType *type, llvm::DataLayout*& dl);

    const int getPointerLevel() override;

    /**
     * Returns the pointer object for the member at index memberIdx if this index corresponds to a pointer type member.
     * @param memberIdx the index of the desired pointer member.
     * @return the pointer object for the member at index memberIdx if this index corresponds to a pointer type member.
     * @throws MemberNotFoundExeption if the given index does not correspond to a pointer member.
     */
    AbstractPointer* getPointerMember(int memberIdx);

    /**
     * Returns the composite object for the member at index memberIdx if this index corresponds to a composite type member.
     * @param memberIdx the index of the desired composite member.
     * @return the composite object for the member at index memberIdx if this index corresponds to a composite type member.
     * @throws MemberNotFoundExeption if the given index does not correspond to a composite member.
     */
    CompositeRef* getCompositeMember(int memberIdx);


    /**
     * Generates and returns a target to the member at the given index.
     * @param memberIdx the index of the member to target.
     * @return an AbstractTarget object targeting the member at memberIdx.
     */
    AbstractTarget getMemberTarget(int memberIdx);


    int getMemberCount();

    std::set<AbstractPointer*> getAllPointerMembers();
    std::set<CompositeRef*> getAllCompositeMembers();

    std::set<AbstractPointer *> getAllPointerMembersRecursively();
};

class MemberNotFoundException : public std::exception {

};

class NoCompositeTypeException : public std::exception {

};


#endif //AUA_COMPOSITEREF_H
