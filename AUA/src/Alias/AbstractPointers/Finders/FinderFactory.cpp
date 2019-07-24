//
// Created by mlaupichler on 17.07.19.
//

#include <llvm/IR/Instructions.h>
#include <AUA/Alias/AbstractPointers/GlobalConfiguration.h>
#include "AUA/Alias/AbstractPointers/Finders/FinderFactory.h"

// POINTER FINDING -------------------------------------------------------------

PointerFinder *FinderFactory::getPointerFinder(llvm::Value *value) {


    if (!llvm::isa<llvm::PointerType>(value->getType())) throw NotAPointerException();

    if (auto argument = llvm::dyn_cast<llvm::Argument>(value)) return getBasePointerFinder(argument);
    if (auto allocaInst = llvm::dyn_cast<llvm::AllocaInst>(value)) return getBasePointerFinder(allocaInst);
    if (auto loadInst = llvm::dyn_cast<llvm::LoadInst>(value)) return getNestedPointerFinder(loadInst);
    if (auto gepInst = llvm::dyn_cast<llvm::GetElementPtrInst>(value)) return getMemberPointerFinder(gepInst);
    if (auto callInst = llvm::dyn_cast<llvm::CallInst>(value)) return getReturnedPointerFinder(callInst);

    throw UnknownFinderInstructionException();

}

FromPointerPointerFinder *FinderFactory::getNestedPointerFinder(llvm::LoadInst *loadInst) {

    llvm::Type* inType = loadInst->getType();

    PointerFormat format = getExpectedPointerFormat(inType);

    std::list<llvm::LoadInst *> loadInstructions;
    llvm::Value *value;
    do {
        loadInstructions.push_front(loadInst);
        value = loadInst->getPointerOperand();

        loadInst = llvm::dyn_cast<llvm::LoadInst>(value);
    } while (loadInst);

    int derefDepth = loadInstructions.size() - 1;


    return new FromPointerPointerFinder(getPointerFinder(value), derefDepth, format);

}

BasePointerFinder *FinderFactory::getBasePointerFinder(llvm::AllocaInst *allocaInst) {

    llvm::Type* inType = allocaInst->getAllocatedType();
    PointerFormat format = getExpectedPointerFormat(inType);

    std::string basePointerName = allocaInst->getName();
    return new BasePointerFinder(basePointerName, format);

}

BasePointerFinder *FinderFactory::getBasePointerFinder(llvm::Argument *argument) {

    llvm::Type* inType = argument->getType();

    std::string basePointerName = argument->getName();
    return new BasePointerFinder(basePointerName, getExpectedPointerFormat(inType));

}

MemberPointerFinder *FinderFactory::getMemberPointerFinder(llvm::GetElementPtrInst *gepInst) {

    llvm::Type* inType = gepInst->getResultElementType();

    int memberIndex = getMemberIdx(gepInst);

    CompositeFinder *parentCompFinder = getCompositeFinder(gepInst->getPointerOperand());

    return new MemberPointerFinder(parentCompFinder, memberIndex, getExpectedPointerFormat(inType));

}


ReturnedPointerFinder *FinderFactory::getReturnedPointerFinder(llvm::CallInst *callInst) {

    llvm::Type* inType = callInst->getType();
    PointerFormat format = getExpectedPointerFormat(inType);


    std::map<int, PointerFinder *> pointerParamFinders;
    std::map<int, CompositeFinder *> compositeParamFinders;

    for (int i = 0; i < callInst->getFunctionType()->getNumParams(); ++i) {

        llvm::Value *param = callInst->getArgOperand(i);

        if (param->getType()->isPointerTy()) {

            PointerFinder* ptrFinder = getPointerFinder(param);

            assert(ptrFinder != nullptr);

            pointerParamFinders[i] = ptrFinder;

        } else if (param->getType()->isStructTy() || param->getType()->isArrayTy()) {

            compositeParamFinders[i] = getCompositeFinder(param);
        }
    }


    llvm::Function *function = callInst->getCalledFunction();
    FunctionFinder* funcFinder = getFunctionFinder(function);

    return new ReturnedPointerFinder(funcFinder, pointerParamFinders, compositeParamFinders, format);

}

PointerFormat FinderFactory::getExpectedPointerFormat(llvm::Type* type) {

    assert(llvm::isa<llvm::PointerType>(type));
    llvm::PointerType *expectedPtrType = llvm::cast<llvm::PointerType>(type);
    return PointerFormat(expectedPtrType);

}

// COMPOSITE FINDING -----------------------------------------------------------------

CompositeFinder *FinderFactory::getCompositeFinder(llvm::Value *value) {

    if (!(value->getType()->isArrayTy() || value->getType()->isStructTy())) throw NotACompositeException();

    if (auto allocaInst = llvm::dyn_cast<llvm::AllocaInst>(value)) return getBaseCompositeFinder(allocaInst);
    if (auto loadInst = llvm::dyn_cast<llvm::LoadInst>(value)) return getFromPointerCompositeFinder(loadInst);
    if (auto gepInst = llvm::dyn_cast<llvm::GetElementPtrInst>(value)) return getMemberCompositeFinder(gepInst);
    //if (auto callInst = llvm::dyn_cast<llvm::CallInst>(value)) return getReturnedCompositeFinder(callInst);

    throw UnknownFinderInstructionException();

}

BaseCompositeFinder *FinderFactory::getBaseCompositeFinder(llvm::AllocaInst *allocaInst) {

    std::string baseCompName = allocaInst->getName();

    auto compositeType = llvm::cast<llvm::CompositeType>(allocaInst->getType());
    CompositeFormat expectedFormat = CompositeFormat(compositeType, dl);

    return new BaseCompositeFinder(baseCompName, expectedFormat);

}

FromPointerCompositeFinder *FinderFactory::getFromPointerCompositeFinder(llvm::LoadInst *loadInst) {

    PointerFinder *ptrFinder = getPointerFinder(loadInst->getPointerOperand());

    auto compositeType = llvm::cast<llvm::CompositeType>(loadInst->getType());
    CompositeFormat expectedFormat = CompositeFormat(compositeType, dl);

    return new FromPointerCompositeFinder(ptrFinder, expectedFormat);

}

MemberCompositeFinder *FinderFactory::getMemberCompositeFinder(llvm::GetElementPtrInst *gepInst) {

    std::list<int> memberIndices;
    llvm::Value *value;
    do {

        int memberIdx = getMemberIdx(gepInst);
        memberIndices.push_front(memberIdx);

        value = gepInst->getPointerOperand();
        gepInst = llvm::dyn_cast<llvm::GetElementPtrInst>(value);
    } while (gepInst);

    CompositeFinder *topLevelCompositeFinder = getCompositeFinder(value);

    auto compositeType = llvm::cast<llvm::CompositeType>(gepInst->getType());
    CompositeFormat expectedFormat = CompositeFormat(compositeType, dl);

    return new MemberCompositeFinder(topLevelCompositeFinder, expectedFormat, memberIndices);

}

// TARGET FINDING ------------------------------------------------------------------------

TargetFinder *FinderFactory::getTargetFinder(llvm::Value *value) {

    if (auto allocaInst = llvm::dyn_cast<llvm::AllocaInst>(value)) return getBaseTargetFinder(allocaInst);
    if (auto loadInst = llvm::dyn_cast<llvm::LoadInst>(value)) return getFromPointerTargetFinder(loadInst);
    if (auto gepInst = llvm::dyn_cast<llvm::GetElementPtrInst>(value)) return getMemberTargetFinder(gepInst);

    throw UnknownFinderInstructionException();
}

BaseTargetFinder *FinderFactory::getBaseTargetFinder(llvm::AllocaInst *allocaInst) {

    std::string varName = allocaInst->getName();
    return new BaseTargetFinder(varName);

}

FromPointerTargetFinder *FinderFactory::getFromPointerTargetFinder(llvm::LoadInst *loadInst) {

    PointerFinder *ptrFinder = getPointerFinder(loadInst->getPointerOperand());

    return new FromPointerTargetFinder(ptrFinder);

}

MemberTargetFinder *FinderFactory::getMemberTargetFinder(llvm::GetElementPtrInst *gepInst) {

    int memberIndex = getMemberIdx(gepInst);

    CompositeFinder *parentCompFinder = getCompositeFinder(gepInst->getPointerOperand());

    return new MemberTargetFinder(parentCompFinder, memberIndex);

}


// FUNCTION FINDING ------------------------------------------------------------------------


FunctionFinder *FinderFactory::getFunctionFinder(llvm::Value *value) {


    if (!llvm::isa<llvm::Function>(value)) throw NotAFunctionException();

    if (auto globalValue = llvm::dyn_cast<llvm::GlobalValue>(value)) return getGlobalFunctionFinder(globalValue);

    throw UnknownFinderInstructionException();
}




GlobalFunctionFinder *FinderFactory::getGlobalFunctionFinder(llvm::GlobalValue *globalValue) {

    return new GlobalFunctionFinder(globalValue->getName());

}

// OTHER METHODS ---------------------------------------------------------------------------

int FinderFactory::getMemberIdx(llvm::GetElementPtrInst *gepInst) {

    int opCount = gepInst->getNumOperands();
    auto *CI = llvm::cast<llvm::ConstantInt>(gepInst->getOperand(opCount - 1));
    assert(CI != nullptr);
    int memberIdx = CI->getZExtValue();

    return memberIdx;

}

FinderFactory::FinderFactory(llvm::DataLayout *dl) : dl(dl) {}
