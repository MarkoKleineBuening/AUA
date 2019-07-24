//
// Created by mlaupichler on 20.07.19.
//

#ifndef AUA_COMPOSITEFORMAT_H
#define AUA_COMPOSITEFORMAT_H


#include <AUA/Alias/AbstractPointers/PointerFormat.h>
#include <map>
#include <vector>
#include <llvm/IR/DataLayout.h>

struct CompositeFormat {

public:

    int memberCount = 0;
    int totalSize = 0;
    std::vector<int> offsets;

    std::map<int, PointerFormat> pointerMemberFormats;
    std::map<int, CompositeFormat> compositeMemberFormats;

    bool operator==(const CompositeFormat &rhs) const;

    bool operator!=(const CompositeFormat &rhs) const;

    CompositeFormat();

    CompositeFormat(llvm::CompositeType *compType, llvm::DataLayout *&dl);

};


struct NoCompositeTypeException : public std::exception {

};


#endif //AUA_COMPOSITEFORMAT_H
