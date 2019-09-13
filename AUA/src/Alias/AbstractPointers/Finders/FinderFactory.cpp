//
// Created by mlaupichler on 17.07.19.
//

#include <llvm/IR/Instructions.h>
#include <AUA/Alias/AbstractPointers/GlobalConfiguration.h>
#include <AUA/Alias/AbstractPointers/Finders/AnonymousPointerFinder.h>
#include <AUA/Alias/AbstractPointers/Finders/IndirectCallPointerFinder.h>
#include "AUA/Alias/AbstractPointers/Finders/FinderFactory.h"

// POINTER FINDING -------------------------------------------------------------

PointerFinder *FinderFactory::getPointerFinder(llvm::Value *value, bool isAdress) {

    llvm::Type* expectedType = value->getType();

    if (!(llvm::isa<llvm::PointerType>(expectedType))) throw NotAPointerException();

    if (isAdress) {
        expectedType = llvm::cast<llvm::PointerType>(expectedType)->getElementType();
        if (!(llvm::isa<llvm::PointerType>(expectedType))) throw NotAPointerException();
    }

    PointerFormat expectedFormat = getPointerFormat(expectedType);

    if (auto argument = llvm::dyn_cast<llvm::Argument>(value)) return getBasePointerFinder(argument, expectedFormat, isAdress);
    if (auto allocaInst = llvm::dyn_cast<llvm::AllocaInst>(value)) return getBasePointerFinder(allocaInst, expectedFormat, isAdress);
    if (auto globalVariable = llvm::dyn_cast<llvm::GlobalVariable>(value)) return getGlobalPointerFinder(globalVariable, expectedFormat, isAdress);
    if (auto loadInst = llvm::dyn_cast<llvm::LoadInst>(value)) return getNestedPointerFinder(loadInst, expectedFormat, isAdress);
    if (auto gepInst = llvm::dyn_cast<llvm::GetElementPtrInst>(value)) return getMemberPointerFinder(gepInst, expectedFormat, isAdress);
    if (auto callInst = llvm::dyn_cast<llvm::CallInst>(value)) return getCallPointerFinder(callInst, expectedFormat,
                                                                                           isAdress);
    if(auto bitCastInst = llvm::dyn_cast<llvm::BitCastInst>(value)) return getBitCastPointerFinder(bitCastInst, isAdress);

    throw UnknownFinderInstructionException();

}

FromPointerPointerFinder *FinderFactory::getNestedPointerFinder(llvm::LoadInst *loadInst, PointerFormat expectedFormat, bool isAdress) {

    llvm::Type* inType = loadInst->getType();
    if (!(llvm::isa<llvm::PointerType>(inType))) throw NotAPointerException();

    int derefDepth = 0;

    if (isAdress) {

        inType = llvm::cast<llvm::PointerType>(inType)->getElementType();
        if (!(llvm::isa<llvm::PointerType>(inType))) throw NotAPointerException();
        ++derefDepth;

    }

    PointerFormat actualFormat = getPointerFormat(inType);
    if (actualFormat != expectedFormat) throw PointerFinderConstructionFormatException(loadInst, expectedFormat,
                                                                                       actualFormat);

    std::list<const llvm::LoadInst *> loadInstructions;
    llvm::Value *value;
    do {
        loadInstructions.push_front(loadInst);
        value = loadInst->getPointerOperand();

        loadInst = llvm::dyn_cast<llvm::LoadInst>(value);
    } while (loadInst);

    derefDepth += loadInstructions.size() - 1;


    return new FromPointerPointerFinder(getPointerFinder(value, true), derefDepth, actualFormat,
                                        loadInstructions);

}

PointerFinder * FinderFactory::getGlobalPointerFinder(llvm::GlobalVariable *variable, PointerFormat expectedFormat,
                                                      bool isAdress) {

    llvm::Type* inType = variable->getValueType();
    PointerFormat actualFormat = getPointerFormat(inType);


    if ((!isAdress) && actualFormat.level == expectedFormat.level - 1) {

        // Implicit Referencing of lower level target in llvm IR => Explicit Referencing by anonymous pointer here

        return getAnonymousPointerFinder(variable, expectedFormat, isAdress);

    }

    if (actualFormat == expectedFormat) {

        std::string basePointerName = variable->getName();
        return new GlobalPointerFinder(basePointerName, actualFormat);
    }

    throw PointerFinderConstructionFormatException(variable, expectedFormat, actualFormat);

}

PointerFinder * FinderFactory::getBasePointerFinder(llvm::AllocaInst *allocaInst, PointerFormat expectedFormat,
                                                    bool isAdress) {

    llvm::Type* inType = allocaInst->getAllocatedType();
    PointerFormat actualFormat = getPointerFormat(inType);

    if ((!isAdress) && actualFormat.level == expectedFormat.level - 1) {

        // Implicit Referencing of lower level target in llvm IR => Explicit Referencing by anonymous pointer here

        return getAnonymousPointerFinder(allocaInst, expectedFormat, isAdress);

    }

    if (actualFormat == expectedFormat) {

        std::string basePointerName = allocaInst->getName();
        return new BasePointerFinder(basePointerName, actualFormat, allocaInst);
    }

    throw PointerFinderConstructionFormatException(allocaInst, expectedFormat, actualFormat);

}

PointerFinder * FinderFactory::getBasePointerFinder(llvm::Argument *argument, PointerFormat expectedFormat,
                                                    bool isAdress) {

    llvm::Type* inType = argument->getType();
    PointerFormat actualFormat = getPointerFormat(inType);

    if ((!isAdress) && actualFormat.level == expectedFormat.level - 1) {

        // Implicit Referencing of lower level target in llvm IR => Explicit Referencing by anonymous pointer here

        return getAnonymousPointerFinder(argument, expectedFormat, isAdress);

    }

    if (actualFormat == expectedFormat) {

        std::string basePointerName = argument->getName();
        return new BasePointerFinder(basePointerName, actualFormat);
    }

    throw PointerFinderConstructionFormatException(argument, expectedFormat, actualFormat);

}

AnonymousPointerFinder* FinderFactory::getAnonymousPointerFinder(llvm::AllocaInst* allocaInst, PointerFormat expectedFormat, bool isAdress) {

    assert(!isAdress);

    return new AnonymousPointerFinder(expectedFormat, getBaseTargetFinder(allocaInst));


}

AnonymousPointerFinder* FinderFactory::getAnonymousPointerFinder(llvm::Argument* argument, PointerFormat expectedFormat, bool isAdress) {

    assert(!isAdress);

    return new AnonymousPointerFinder(expectedFormat, getBaseTargetFinder(argument));

}

AnonymousPointerFinder* FinderFactory::getAnonymousPointerFinder(llvm::GetElementPtrInst* gepInst, PointerFormat expectedFormat, bool isAdress) {

    assert(!isAdress);

    return new AnonymousPointerFinder(expectedFormat, getMemberTargetFinder(gepInst));

}


AnonymousPointerFinder *FinderFactory::getAnonymousPointerFinder(llvm::GlobalVariable *variable, PointerFormat expectedFormat, bool isAdress) {

    assert(!isAdress);

    return new AnonymousPointerFinder(expectedFormat, getGlobalTargetFinder(variable));

}

PointerFinder *FinderFactory::getMemberPointerFinder(llvm::GetElementPtrInst *gepInst, PointerFormat expectedFormat, bool isAdress) {

    llvm::Type* inType = gepInst->getResultElementType();
    PointerFormat actualFormat = getPointerFormat(inType);

    if ((!isAdress) && actualFormat.level == expectedFormat.level - 1) {

        // Implicit Referencing of lower level target in llvm IR => Explicit Referencing by anonymous pointer here

        return getAnonymousPointerFinder(gepInst, expectedFormat, isAdress);

    }

    if (actualFormat == expectedFormat) {

        int memberIndex = getMemberIdx(gepInst);
        CompositeFinder *parentCompFinder = getCompositeFinder(gepInst->getPointerOperand());

        return new MemberPointerFinder(parentCompFinder, memberIndex, getPointerFormat(inType), gepInst);
    }

    throw PointerFinderConstructionFormatException(gepInst, expectedFormat, actualFormat);



}

CallPointerFinder * FinderFactory::getCallPointerFinder(llvm::CallInst *callInst, PointerFormat expectedFormat,
                                                        bool isAdress) {

    llvm::Type* inType = callInst->getType();
    PointerFormat actualFormat = getPointerFormat(inType);
    if (actualFormat != expectedFormat) throw PointerFinderConstructionFormatException(callInst, expectedFormat,
                                                                                       actualFormat);

    if (callInst->isIndirectCall()) {

        //Ignore indirect calls => Return pointer finder that returns dummy pointer.

        return new IndirectCallPointerFinder(actualFormat, callInst);
    }


    std::map<int, PointerFinder *> pointerParamFinders;
    std::map<int, CompositeFinder *> compositeParamFinders;

    for (int i = 0; i < callInst->getFunctionType()->getNumParams(); ++i) {

        llvm::Value *param = callInst->getArgOperand(i);

        if (param->getType()->isPointerTy()) {

            PointerFinder* ptrFinder = getPointerFinder(param, false);

            assert(ptrFinder != nullptr);

            pointerParamFinders[i] = ptrFinder;

        } else if (param->getType()->isStructTy() || param->getType()->isArrayTy()) {

            compositeParamFinders[i] = getCompositeFinder(param);
        }
    }


    llvm::Function *function = callInst->getCalledFunction();
    FunctionFinder* funcFinder = getFunctionFinder(function);

    return new DirectCallPointerFinder(funcFinder, pointerParamFinders, compositeParamFinders, actualFormat, callInst);

}

PointerFinder *FinderFactory::getBitCastPointerFinder(llvm::BitCastInst *bitCastInst, bool isAdress) {

    llvm::Value* sourceValue = bitCastInst->getOperand(0);
    return getPointerFinder(sourceValue, isAdress);

}

PointerFormat FinderFactory::getPointerFormat(llvm::Type *type) {

    return PointerFormat(type);

}

// COMPOSITE FINDING -----------------------------------------------------------------

CompositeFinder *FinderFactory::getCompositeFinder(llvm::Value *value) {

    if (auto allocaInst = llvm::dyn_cast<llvm::AllocaInst>(value)) return getBaseCompositeFinder(allocaInst);
    if (auto globalVariable = llvm::dyn_cast<llvm::GlobalVariable>(value)) return getGlobalCompositeFinder(globalVariable);
    if (auto loadInst = llvm::dyn_cast<llvm::LoadInst>(value)) return getFromPointerCompositeFinder(loadInst);
    if (auto gepInst = llvm::dyn_cast<llvm::GetElementPtrInst>(value)) return getMemberCompositeFinder(gepInst);
    //if (auto callInst = llvm::dyn_cast<llvm::CallInst>(value)) return getReturnedCompositeFinder(callInst);

    throw UnknownFinderInstructionException();

}

BaseCompositeFinder *FinderFactory::getBaseCompositeFinder(llvm::AllocaInst *allocaInst) {

    llvm::outs() << "Constructing BaseCompositeFinder.\n";

    std::string baseCompName = allocaInst->getName();

    auto compositeType = llvm::cast<llvm::CompositeType>(allocaInst->getAllocatedType());
    CompositeFormat expectedFormat = CompositeFormat(compositeType, dl);

    return new BaseCompositeFinder(baseCompName, expectedFormat, allocaInst);

}

GlobalCompositeFinder *FinderFactory::getGlobalCompositeFinder(llvm::GlobalVariable* variable) {

    llvm::outs() << "Constructing GlobalCompositeFinder.\n";

    std::string compName = variable->getName();
    auto compType = llvm::cast<llvm::CompositeType>(variable->getValueType());

    return new GlobalCompositeFinder(compName, CompositeFormat(compType, dl));

}

FromPointerCompositeFinder *FinderFactory::getFromPointerCompositeFinder(llvm::LoadInst *loadInst) {

    llvm::outs() << "Constructing FromPointerCompositeFinder.\n";
    llvm::outs() << "Pointer Name: " << loadInst->getPointerOperand()->getName() << "\n";

    PointerFinder *ptrFinder = getPointerFinder(loadInst->getPointerOperand(), true);

    assert(loadInst->getType()->isPointerTy());
    auto ptrType = llvm::cast<llvm::PointerType>(loadInst->getType());

    auto compositeType = llvm::cast<llvm::CompositeType>(ptrType->getElementType());
    CompositeFormat expectedFormat = CompositeFormat(compositeType, dl);


    return new FromPointerCompositeFinder(ptrFinder, expectedFormat, loadInst);

}

MemberCompositeFinder *FinderFactory::getMemberCompositeFinder(llvm::GetElementPtrInst *gepInst) {

    llvm::outs() << "Constructing MemberCompositeFinder.\n";

    auto compositeType = llvm::cast<llvm::CompositeType>(gepInst->getResultElementType());
    CompositeFormat actualFormat = CompositeFormat(compositeType, dl);


    std::list<int> memberIndices;
    std::list<llvm::GetElementPtrInst*> gepInsts;
    llvm::Value *value;
    do {

        int memberIdx = getMemberIdx(gepInst);
        memberIndices.push_front(memberIdx);

        gepInsts.push_front(gepInst);

        value = gepInst->getPointerOperand();
        gepInst = llvm::dyn_cast<llvm::GetElementPtrInst>(value);
    } while (gepInst);

    CompositeFinder *topLevelCompositeFinder = getCompositeFinder(value);

    return new MemberCompositeFinder(topLevelCompositeFinder, actualFormat, memberIndices,
                                     gepInsts);

}

// TARGET FINDING ------------------------------------------------------------------------

TargetFinder *FinderFactory::getTargetFinder(llvm::Value *value) {

    if (auto allocaInst = llvm::dyn_cast<llvm::AllocaInst>(value)) return getBaseTargetFinder(allocaInst);
    if (auto globalVariable = llvm::dyn_cast<llvm::GlobalVariable>(value)) return getGlobalTargetFinder(globalVariable);
    if (auto loadInst = llvm::dyn_cast<llvm::LoadInst>(value)) return getFromPointerTargetFinder(loadInst);
    if (auto gepInst = llvm::dyn_cast<llvm::GetElementPtrInst>(value)) return getMemberTargetFinder(gepInst);

    throw UnknownFinderInstructionException();
}


BaseTargetFinder *FinderFactory::getBaseTargetFinder(llvm::AllocaInst *allocaInst) {

    std::string varName = allocaInst->getName();
    return new BaseTargetFinder(varName, allocaInst);

}


BaseTargetFinder *FinderFactory::getBaseTargetFinder(llvm::Argument *argument) {

    std::string varName = argument->getName();
    return new BaseTargetFinder(varName);

}




GlobalTargetFinder *FinderFactory::getGlobalTargetFinder(llvm::GlobalVariable *variable) {

    std::string varName = variable->getName();
    return new GlobalTargetFinder(varName);

}

FromPointerTargetFinder *FinderFactory::getFromPointerTargetFinder(llvm::LoadInst *loadInst) {

    PointerFinder *ptrFinder = getPointerFinder(loadInst->getPointerOperand(), true);

    return new FromPointerTargetFinder(ptrFinder, loadInst);

}

MemberTargetFinder *FinderFactory::getMemberTargetFinder(llvm::GetElementPtrInst *gepInst) {

    int memberIndex = getMemberIdx(gepInst);

    CompositeFinder *parentCompFinder = getCompositeFinder(gepInst->getPointerOperand());

    return new MemberTargetFinder(parentCompFinder, memberIndex, gepInst);

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
