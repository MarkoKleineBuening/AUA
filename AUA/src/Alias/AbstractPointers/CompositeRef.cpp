//
// Created by mlaupichler on 20.06.19.
//

#include <llvm/IR/DerivedTypes.h>
#include "AUA/Alias/AbstractPointers/CompositeRef.h"

const int CompositeRef::getPointerLevel() {
    return 0;
}

AbstractPointer *CompositeRef::getPointerMember(int memberIdx) {

    if (memberIdx >= memberCount) throw  MemberNotFoundException();

    if (memberMeta[memberIdx]->type != PTR)
        throw  MemberNotFoundException();

    // Generate pointer object lazily
    if (pointers.find(memberIdx) == pointers.end()) {
        AbstractPointer* ptr = new AbstractPointer(generateMemberName(memberIdx), 0, memberMeta[memberIdx]->pointerLevel);
        pointers[memberIdx] = ptr;
    }

    return pointers[memberIdx];

}

CompositeRef *CompositeRef::getCompositeMember(int memberIdx) {


    if (memberIdx >= memberCount) throw  MemberNotFoundException();

    if (memberMeta[memberIdx]->type != COMP)
        throw  MemberNotFoundException();

    if (composites.find(memberIdx) == composites.end()) {
        throw  MemberNotFoundException();
    }

    return composites[memberIdx];

}

std::string CompositeRef::generateMemberName(int idx) {
    return getName() + "[" + std::to_string(idx) + "]";
}

AbstractTarget CompositeRef::getMemberTarget(int memberIdx) {

    if (memberIdx >= memberCount) throw  MemberNotFoundException();

    MemberMetaInfo* metaInfo = memberMeta[memberIdx];


        switch (metaInfo->type) {
            case VAR: {

                return AbstractTarget(this, metaInfo->byteOffset, metaInfo->size);

            }

            case PTR: {

                AbstractPointer *ptr = getPointerMember(memberIdx);

                return AbstractTarget(ptr, 0, metaInfo->size);

            }

            case COMP: {

                CompositeRef *comp = getCompositeMember(memberIdx);

                return AbstractTarget(comp, 0, metaInfo->size);

            }

        }


    throw MemberNotFoundException();


}

CompositeRef::CompositeRef(const std::string name, int alignment, llvm::CompositeType *type, llvm::DataLayout*& dl)
        : AbstractReference(name, alignment) {

    if (llvm::ArrayType* arrayType = llvm::dyn_cast<llvm::ArrayType>(type)) {
        memberCount = arrayType->getNumElements();
    } else if (llvm::StructType* structType = llvm::dyn_cast<llvm::StructType>(type)) {
        memberCount = structType->getNumElements();
    } else {
        throw NoCompositeTypeException();
    }

    memberMeta.reserve(memberCount);

    int os = 0;

    for (int i = 0; i < memberCount; ++i) {

        llvm::Type* memberType = type->getTypeAtIndex(i);
        MemberMetaInfo* metaInfo = getMetaForType(memberType, os, dl);
        os += metaInfo->size;

        memberMeta.push_back(metaInfo);
        assert(memberMeta.size() == i + 1);

        if (llvm::CompositeType* compType = llvm::dyn_cast<llvm::CompositeType>(memberType)) {

            CompositeRef* compMember = new CompositeRef(generateMemberName(i), 0, compType, dl);
            composites[i] = compMember;

        }

    }

    totalSize = os;

    if (type->isSized()) {
        assert(totalSize == (dl->getTypeAllocSize(type)));
    }



}

MemberMetaInfo *CompositeRef::getMetaForType(llvm::Type *type, int offset, llvm::DataLayout *dl) {

    int size = dl->getTypeAllocSize(type);

    MemberType t = VAR;
    int ptrLevel = 0;

    if (type->isPointerTy()) {

        t = PTR;
        while(type->isPointerTy()) {

            llvm::PointerType* ptrType = llvm::dyn_cast<llvm::PointerType>(type);
            assert(ptrType != nullptr);
            type = ptrType->getElementType();
            ++ptrLevel;
        }

    } else if (llvm::CompositeType* compType = llvm::dyn_cast<llvm::CompositeType>(type)) {

        t = COMP;

    }


    MemberMetaInfo* info = new MemberMetaInfo(size, offset, ptrLevel, t);

    return info;

}

int CompositeRef::getMemberCount() {
    return memberCount;
}

std::set<AbstractPointer *> CompositeRef::getAllPointerMembers() {

    std::set<AbstractPointer*> result;

    for (auto ptrPair : pointers) {
        result.insert(ptrPair.second);
    }

    return result;


}

MemberMetaInfo::MemberMetaInfo(const int size, const int byteOffset, const int pointerLevel, const MemberType type)
        : size(size), byteOffset(byteOffset), pointerLevel(pointerLevel), type(type) {}
