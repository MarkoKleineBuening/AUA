//
// Created by mlaupichler on 17.07.19.
//

#ifndef AUA_FINDERFACTORY_H
#define AUA_FINDERFACTORY_H


#include <llvm/IR/Value.h>
#include <llvm/IR/Instructions.h>
#include "PointerFinder.h"
#include "BasePointerFinder.h"
#include "FromPointerPointerFinder.h"
#include "MemberPointerFinder.h"
#include "ReturnedPointerFinder.h"
#include "CompositeFinder.h"
#include "BaseCompositeFinder.h"
#include "FromPointerCompositeFinder.h"
#include "MemberCompositeFinder.h"
#include "TargetFinder.h"
#include "BaseTargetFinder.h"
#include "FromPointerTargetFinder.h"
#include "MemberTargetFinder.h"
#include "GlobalFunctionFinder.h"

class FinderFactory {

private:

    llvm::DataLayout *dl;


    static BasePointerFinder *getBasePointerFinder(llvm::AllocaInst *allocaInst);
    static BasePointerFinder *getBasePointerFinder(llvm::Argument *argument);

    FromPointerPointerFinder *getNestedPointerFinder(llvm::LoadInst *loadInst);

    MemberPointerFinder *getMemberPointerFinder(llvm::GetElementPtrInst *gepInst);

    ReturnedPointerFinder *getReturnedPointerFinder(llvm::CallInst *callInst);

    static PointerFormat getExpectedPointerFormat(llvm::Type *type);

    BaseCompositeFinder *getBaseCompositeFinder(llvm::AllocaInst *allocaInst);

    FromPointerCompositeFinder *getFromPointerCompositeFinder(llvm::LoadInst *loadInst);

    MemberCompositeFinder *getMemberCompositeFinder(llvm::GetElementPtrInst *gepInst);

    static BaseTargetFinder *getBaseTargetFinder(llvm::AllocaInst *allocaInst);

    FromPointerTargetFinder *getFromPointerTargetFinder(llvm::LoadInst *loadInst);

    MemberTargetFinder *getMemberTargetFinder(llvm::GetElementPtrInst *gepInst);

    GlobalFunctionFinder *getGlobalFunctionFinder(llvm::GlobalValue *globalValue);

    static int getMemberIdx(llvm::GetElementPtrInst *gepInst);

public:

    explicit FinderFactory(llvm::DataLayout *dl);

    PointerFinder *getPointerFinder(llvm::Value *value);

    CompositeFinder *getCompositeFinder(llvm::Value *value);

    TargetFinder *getTargetFinder(llvm::Value *value);

    FunctionFinder *getFunctionFinder(llvm::Value *value);
};

struct NotAPointerException : public std::exception {
    const char *what() const throw() {
        return "The given value is not a pointer.";
    }
};

struct NotACompositeException : public std::exception {
    const char *what() const throw() {
        return "The given value is not a composite (struct or array).";
    }
};

struct NotAFunctionException : public std::exception {
    const char *what() const throw() {
        return "The given value is not a function.";
    }
};

struct UnknownFinderInstructionException : public std::exception {
    const char *what() const throw() {
        return "An unknown instruction was encountered while trying to build a Finder.";
    }
};


#endif //AUA_FINDERFACTORY_H
