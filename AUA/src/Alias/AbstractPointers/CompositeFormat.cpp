//
// Created by mlaupichler on 20.07.19.
//

#include "AUA/Alias/AbstractPointers/CompositeFormat.h"


CompositeFormat::CompositeFormat(llvm::CompositeType *compType, llvm::DataLayout *&dl) {

    if (llvm::ArrayType *arrayType = llvm::dyn_cast<llvm::ArrayType>(compType)) {
        memberCount = arrayType->getNumElements();
    } else if (llvm::StructType *structType = llvm::dyn_cast<llvm::StructType>(compType)) {
        memberCount = structType->getNumElements();
    } else {
        throw NoCompositeTypeException();
    }


    offsets.reserve(memberCount);

    int os = 0;

    for (int i = 0; i < memberCount; ++i) {

        offsets.push_back(os);

        llvm::Type *memberType = compType->getTypeAtIndex(i);

        int size = dl->getTypeAllocSize(memberType);
        os += size;

        if (llvm::PointerType *ptrType = llvm::dyn_cast<llvm::PointerType>(memberType)) {

            PointerFormat format = PointerFormat(ptrType);
            pointerMemberFormats.insert(std::pair(i, format));
        } else if (llvm::CompositeType *compType = llvm::dyn_cast<llvm::CompositeType>(memberType)) {
            CompositeFormat format = CompositeFormat(compType, dl);
            compositeMemberFormats[i] = format;
        }

    }

    totalSize = dl->getTypeAllocSize(compType);
}


bool CompositeFormat::operator==(const CompositeFormat &rhs) const {
    return memberCount == rhs.memberCount &&
           totalSize == rhs.totalSize &&
           offsets == rhs.offsets &&
           pointerMemberFormats == rhs.pointerMemberFormats &&
           compositeMemberFormats == rhs.compositeMemberFormats;
}

bool CompositeFormat::operator!=(const CompositeFormat &rhs) const {
    return !(rhs == *this);
}

CompositeFormat::CompositeFormat() {

    memberCount = 0;
    totalSize = 0;
    offsets = std::vector<int>();
    pointerMemberFormats = std::map<int, PointerFormat>();
    compositeMemberFormats = std::map<int, CompositeFormat>();

}
