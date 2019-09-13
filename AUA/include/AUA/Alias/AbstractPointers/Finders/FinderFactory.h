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
#include "DirectCallPointerFinder.h"
#include "AnonymousPointerFinder.h"
#include "CompositeFinder.h"
#include "BaseCompositeFinder.h"
#include "FromPointerCompositeFinder.h"
#include "MemberCompositeFinder.h"
#include "TargetFinder.h"
#include "BaseTargetFinder.h"
#include "FromPointerTargetFinder.h"
#include "MemberTargetFinder.h"
#include "GlobalFunctionFinder.h"
#include "GlobalPointerFinder.h"
#include "GlobalTargetFinder.h"
#include "GlobalCompositeFinder.h"
#include <llvm/IR/GlobalVariable.h>

class FinderFactory {

private:

    llvm::DataLayout *dl;


    static PointerFinder * getBasePointerFinder(llvm::AllocaInst *allocaInst, PointerFormat expectedFormat, bool isAdress);
    static PointerFinder * getBasePointerFinder(llvm::Argument *argument, PointerFormat expectedFormat, bool isAdress);
    static PointerFinder * getGlobalPointerFinder(llvm::GlobalVariable *variable, PointerFormat expectedFormat,
                                                  bool isAdress);
    PointerFinder *getBitCastPointerFinder(llvm::BitCastInst *bitCastInst, bool isAdress);

    PointerFinder * getMemberPointerFinder(llvm::GetElementPtrInst *gepInst, PointerFormat expectedFormat, bool isAdress);
    static AnonymousPointerFinder *getAnonymousPointerFinder(llvm::AllocaInst *allocaInst, PointerFormat expectedFormat, bool isAdress);
    static AnonymousPointerFinder *getAnonymousPointerFinder(llvm::Argument *argument, PointerFormat expectedFormat, bool isAdress);
    AnonymousPointerFinder *getAnonymousPointerFinder(llvm::GetElementPtrInst *gepInst, PointerFormat expectedFormat, bool isAdress);

    static AnonymousPointerFinder *getAnonymousPointerFinder(llvm::GlobalVariable *variable, PointerFormat expectedFormat, bool isAdress);
    FromPointerPointerFinder *getNestedPointerFinder(llvm::LoadInst *loadInst, PointerFormat expectedFormat, bool isAdress);

    CallPointerFinder * getCallPointerFinder(llvm::CallInst *callInst, PointerFormat expectedFormat, bool isAdress);


    static PointerFormat getPointerFormat(llvm::Type *type);
    BaseCompositeFinder *getBaseCompositeFinder(llvm::AllocaInst *allocaInst);
    GlobalCompositeFinder* getGlobalCompositeFinder(llvm::GlobalVariable* variable);
    FromPointerCompositeFinder *getFromPointerCompositeFinder(llvm::LoadInst *loadInst);


    MemberCompositeFinder *getMemberCompositeFinder(llvm::GetElementPtrInst *gepInst);
    static BaseTargetFinder *getBaseTargetFinder(llvm::AllocaInst *allocaInst);
    static BaseTargetFinder *getBaseTargetFinder(llvm::Argument *argument);
    static GlobalTargetFinder *getGlobalTargetFinder(llvm::GlobalVariable *variable);
    FromPointerTargetFinder *getFromPointerTargetFinder(llvm::LoadInst *loadInst);


    MemberTargetFinder *getMemberTargetFinder(llvm::GetElementPtrInst *gepInst);

    static GlobalFunctionFinder *getGlobalFunctionFinder(llvm::GlobalValue *globalValue);

    static int getMemberIdx(llvm::GetElementPtrInst *gepInst);

public:

    explicit FinderFactory(llvm::DataLayout *dl);

    PointerFinder *getPointerFinder(llvm::Value *value, bool isAdress);

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

struct PointerFinderConstructionFormatException : public std::exception {

private:

    llvm::Value* value;
    PointerFormat actualFormat;
    PointerFormat expectedFormat;

public:

    PointerFinderConstructionFormatException(llvm::Value *value, const PointerFormat &expectedFormat,
                                             const PointerFormat &actualFormat) : value(value),
                                                                                    actualFormat(actualFormat),
                                                                                    expectedFormat(expectedFormat) {}

    const char *what() const throw() {

        std::ostringstream oss;
        oss << "The return format of the instruction \" " << value->getName().data() << " \" does not match the expected format. ";
        oss << "(actual level = " << actualFormat.level << " != " << expectedFormat.level << " = expected level)!\n";
        auto msg = oss.str();
        char* chars = (char*) std::malloc((msg.length() + 1) * sizeof(char));
        std::strcpy(chars, msg.c_str());
        return chars;
    }
};







#endif //AUA_FINDERFACTORY_H
