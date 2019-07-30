//
// Created by mlaupichler on 10.07.19.
//

#include <AUA/Alias/AbstractOps/AssignmentOp.h>
#include <AUA/Alias/AbstractOps/StoreOp.h>
#include <AUA/Alias/AbstractPointers/Finders/MemberTargetFinder.h>
#include <AUA/Alias/AbstractPointers/Finders/BaseTargetFinder.h>
#include <AUA/Alias/AbstractPointers/Finders/MemberPointerFinder.h>
#include <AUA/Alias/AbstractPointers/Finders/BasePointerFinder.h>
#include <AUA/Alias/AbstractOps/VarAllocationOp.h>
#include <AUA/Alias/AbstractOps/CompositeAllocationOp.h>
#include <AUA/Alias/AbstractOps/PointerAllocationOp.h>
#include <AUA/Alias/AbstractOps/SplitOp.h>
#include <AUA/Alias/AbstractPointers/Finders/MemberCompositeFinder.h>
#include <AUA/Alias/AbstractPointers/Finders/BaseCompositeFinder.h>
#include <AUA/Alias/AbstractOps/ReturnPointerOp.h>
#include <AUA/Alias/AbstractOps/ReturnNilOp.h>
#include <AUA/Alias/AbstractOps/CallWithIrrelevantReturnOp.h>
#include "AUA/Alias/AbstractPointers/AbstractFunctionFactory.h"
#include "../../../../tools/src/PointerIrrelevantException.h"

AbstractFunctionFactory::AbstractFunctionFactory(llvm::DataLayout *dataLayout, FinderFactory *finderFactory)
        : dataLayout(dataLayout), finderFactory(finderFactory) {

    resetAbstractionState();

}

void AbstractFunctionFactory::resetAbstractionState() {

    initialOp = nullptr;
    finalOp = nullptr;

    loopLatchFound.clear();
    loopJoinOps.clear();
    ifJoinOps.clear();

}

AbstractFunction *AbstractFunctionFactory::buildAbstractFunction(llvm::Function *function) {

    resetAbstractionState();

    std::string name = function->getName();

    initialOp = new DummyInitialOp();


    PointerOperation *firstOp = abstractBasicBlock(&function->getEntryBlock());
    initialOp->linkSuccessor(firstOp);

    std::map<int, PointerFormat>* pointerParamFormats = getPointerParamFormats(function);
    std::map<int, CompositeFormat>* compositeParamFormats = getCompositeParamFormats(function);
    std::set<AbstractVar*>* varParams = getVarParams(function);
    std::vector<std::string> paramNames = getParamNames(function);

    auto resultFunc = new AbstractFunction(initialOp, finalOp, name, pointerParamFormats,
                                           compositeParamFormats, varParams, paramNames);

    resetAbstractionState();
    return resultFunc;

}


std::map<int, PointerFormat> * AbstractFunctionFactory::getPointerParamFormats(llvm::Function *function) {

    auto result = new std::map<int, PointerFormat>();

    for (auto PI = function->arg_begin(), PE = function->arg_end(); PI != PE; ++PI) {

        llvm::Argument *param = PI;
        llvm::Type *type = param->getType();

        if (auto ptrType = llvm::dyn_cast<llvm::PointerType>(type)) {

            result->insert(std::pair(param->getArgNo(), PointerFormat(ptrType)));
        }
    }

    return result;
}

std::map<int, CompositeFormat> * AbstractFunctionFactory::getCompositeParamFormats(llvm::Function *function) {

    auto result = new std::map<int, CompositeFormat>();

    for (auto PI = function->arg_begin(), PE = function->arg_end(); PI != PE; ++PI) {

        llvm::Argument *param = PI;
        llvm::Type *type = param->getType();

        if (auto compType = llvm::dyn_cast<llvm::CompositeType>(type)) {

            int argIndex = param->getArgNo();
            result->insert(std::pair(argIndex, CompositeFormat(compType, dataLayout)));
        }
    }

    return result;

}

std::set<AbstractVar*>* AbstractFunctionFactory::getVarParams(llvm::Function *function) {

    auto result = new std::set<AbstractVar*>();
    ReferenceFlags paramFlags = ReferenceFlags(false, false, true);

    for (auto PI = function->arg_begin(), PE = function->arg_end(); PI != PE; ++PI) {

        llvm::Argument *param = PI;
        llvm::Type *type = param->getType();

        if (!llvm::isa<llvm::CompositeType>(type) && !llvm::isa<llvm::PointerType>(type)) {

            auto var = new AbstractVar(param->getName(), dataLayout->getTypeAllocSize(type), paramFlags);
            result->insert(var);
        }
    }

    return result;
}


std::vector<std::string> AbstractFunctionFactory::getParamNames(llvm::Function *function) {

    std::vector<std::string> paramNames;
    paramNames.reserve(function->getNumOperands());

    for (auto PI = function->arg_begin(), PE = function->arg_end(); PI != PE; ++PI) {

        paramNames.push_back((*PI).getName());
    }

    return paramNames;

}

PointerOperation *AbstractFunctionFactory::abstractBasicBlock(llvm::BasicBlock *BB) {

    llvm::outs() << "\nAbstracting block " << BB->getName() << "\n";


    // Block is loop condition iff it has two predecessors and two successors
    if (BB->hasNPredecessors(2) && (BB->getTerminator()->getNumSuccessors() == 2)) {

        return abstractLoopConditionBlock(BB);


    }

    //Block is if terminated iff it has less than two predecessors and two successors
    if (!BB->hasNPredecessorsOrMore(2) && (BB->getTerminator()->getNumSuccessors() == 2)) {

        return abstractIfBranchBlock(BB);


    }

    //Block is if rejoin iff it has two predecessors and less than two successors
    if (BB->hasNPredecessors(2) && (BB->getTerminator()->getNumSuccessors() < 2)) {

        return abstractIfRejoinBlock(BB);


    }

    PointerOperation *first = nullptr;
    PointerOperation *last = nullptr;

    auto abstractedOpPair = abstractBlockInstructions(BB);
    if (abstractedOpPair != nullptr) {

        first = abstractedOpPair->first;
        last = abstractedOpPair->second;
    }

    PointerOperation *succOp;

    llvm::Instruction *terminator = BB->getTerminator();

    if (terminator->getNumSuccessors() == 0) {

        assert(llvm::isa<llvm::ReturnInst>(terminator));
        auto returnInst = llvm::cast<llvm::ReturnInst>(terminator);

        finalOp = buildFinalOp(returnInst);
        succOp = finalOp;

    } else if (terminator->getNumSuccessors() == 1) {

        succOp = abstractBasicBlock(terminator->getSuccessor(0));

    } else {
        throw new std::exception();
    }


    if (last == nullptr) {
        return succOp;
    }

    last->linkSuccessor(succOp);
    return first;

}

PointerOperation *AbstractFunctionFactory::abstractLoopConditionBlock(llvm::BasicBlock *BB) {

    assert(BB->hasNPredecessors(2));
    assert(BB->getTerminator()->getNumSuccessors() == 2);

    if (loopJoinOps.find(BB->getName()) != loopJoinOps.end()) {

        loopLatchFound[BB->getName()] = true;
        return loopJoinOps[BB->getName()];

    }

    loopLatchFound[BB->getName()] = false;

    PointerOperation *first = nullptr;
    PointerOperation *last = nullptr;

    auto abstractedOpPair = abstractBlockInstructions(BB);
    if (abstractedOpPair != nullptr) {

        first = abstractedOpPair->first;
        last = abstractedOpPair->second;
    }


    SplitOp *splitOp = new SplitOp();
    JoinOp *joinOp = new JoinOp();
    splitOp->linkSuccessor(joinOp);
    loopJoinOps[BB->getName()] = joinOp;

    if (first == nullptr) {
        first = splitOp;
    } else {
        last->linkSuccessor(splitOp);
    }

    PointerOperation *loopBodySuccOp;
    PointerOperation *loopExitSuccOp;

    PointerOperation *firstSuccOp = abstractBasicBlock(BB->getTerminator()->getSuccessor(0));

    if (loopLatchFound[BB->getName()]) {
        loopBodySuccOp = firstSuccOp;
        loopExitSuccOp = abstractBasicBlock(BB->getTerminator()->getSuccessor(1));
    } else {
        loopBodySuccOp = abstractBasicBlock(BB->getTerminator()->getSuccessor(1));
        loopExitSuccOp = firstSuccOp;
    }

    splitOp->linkSuccessor(loopBodySuccOp);
    joinOp->linkSuccessor(loopExitSuccOp);

    return first;

}

PointerOperation *AbstractFunctionFactory::abstractIfBranchBlock(llvm::BasicBlock *BB) {

    assert(!BB->hasNPredecessorsOrMore(2));
    assert(BB->getTerminator()->getNumSuccessors() == 2);

    PointerOperation *first = nullptr;
    PointerOperation *last = nullptr;

    auto abstractedOpPair = abstractBlockInstructions(BB);
    if (abstractedOpPair != nullptr) {

        first = abstractedOpPair->first;
        last = abstractedOpPair->second;
    }

    SplitOp *splitOp = new SplitOp();
    if (first == nullptr) {
        first = splitOp;
    } else {
        last->linkSuccessor(splitOp);
    }

    PointerOperation *firstSuccOp = abstractBasicBlock(BB->getTerminator()->getSuccessor(0));
    splitOp->linkSuccessor(firstSuccOp);
    PointerOperation *secondSuccOp = abstractBasicBlock(BB->getTerminator()->getSuccessor(1));
    splitOp->linkSuccessor(secondSuccOp);

    return first;

}

PointerOperation *AbstractFunctionFactory::abstractIfRejoinBlock(llvm::BasicBlock *BB) {

    assert(BB->hasNPredecessors(2));
    assert(BB->getTerminator()->getNumSuccessors() < 2);

    if (ifJoinOps.find(BB->getName()) != ifJoinOps.end()) {

        return ifJoinOps[BB->getName()];
    }

    PointerOperation *first = nullptr;
    PointerOperation *last = nullptr;

    auto abstractedOpPair = abstractBlockInstructions(BB);
    if (abstractedOpPair != nullptr) {

        first = abstractedOpPair->first;
        last = abstractedOpPair->second;
    }

    JoinOp *joinOp = new JoinOp();
    ifJoinOps[BB->getName()] = joinOp;

    if (first == nullptr) {
        last = joinOp;
    } else {
        joinOp->linkSuccessor(first);
    }

    PointerOperation *succOp;

    llvm::Instruction *terminator = BB->getTerminator();
    if (terminator->getNumSuccessors() == 0) {

        assert(llvm::isa<llvm::ReturnInst>(terminator));
        auto returnInst = llvm::cast<llvm::ReturnInst>(terminator);

        finalOp = buildFinalOp(returnInst);
        succOp = finalOp;

    } else {
        // Only one successor
        succOp = abstractBasicBlock(BB->getTerminator()->getSuccessor(0));
    }

    last->linkSuccessor(succOp);
    return joinOp;


}


/**
 * Abstracts all pointer relevant instructions from BB into PointerOperations and attaches the resulting CFG to the given previous PointerOperation. It ignores all control flow instructions.
 * @param BB the BasicBlock whose instructions to abstract.
 * @param predExitOp the PointerOperation preceeding this block.
 * @return the first and last PointerOperation abstracted from this block if any or nullptr if none were abstracted from this block.
 */
std::pair<PointerOperation *, PointerOperation *> *
AbstractFunctionFactory::abstractBlockInstructions(llvm::BasicBlock *BB) {


    PointerOperation *first = nullptr;
    PointerOperation *last = nullptr;


    for (auto & BBI : *BB) {

        llvm::Instruction *inst = &BBI;

        const char *termMsg = (inst->isTerminator()) ? " (Terminator)" : "";
        llvm::outs() << *inst << termMsg << "\n";

        try {

            PointerOperation *op = abstractInstruction(inst);

            if (first == nullptr) {

                first = op;
            } else {

                last->linkSuccessor(op);
            }

            last = op;

        } catch (PointerIrrelevantException &e) {
            // instruction wasn't relevant for pointers. Continue with next instruction.
        }
    }


    if (first != nullptr && last != nullptr) {
        llvm::outs() << "Block " << BB->getName() << " has PointerOperations.\n";
        return new std::pair<PointerOperation *, PointerOperation *>(first, last);
    }

    llvm::outs() << "Block " << BB->getName() << " has no PointerOperations.\n";
    return nullptr;

}

PointerOperation *AbstractFunctionFactory::abstractInstruction(llvm::Instruction *inst) {

    if (auto *storeInst = llvm::dyn_cast<llvm::StoreInst>(inst)) {

        return handleStore(storeInst);
    }

    if (llvm::CallInst *callInst = llvm::dyn_cast<llvm::CallInst>(inst)) {

        // TODO Change predicate once composite returns are okay
        if (!callInst->getFunctionType()->getReturnType()->isPointerTy()) {

            llvm::outs() << "Non pointer return function found.\n";
            return handleCallWithIrrelevantReturn(callInst);
        }

    }

    if (llvm::AllocaInst *allocaInst = llvm::dyn_cast<llvm::AllocaInst>(inst)) {
        return handleAllocation(allocaInst);
    }

    throw PointerIrrelevantException();

}


PointerOperation *AbstractFunctionFactory::handleAllocation(llvm::AllocaInst *allocaInst) {

    llvm::Type *type = allocaInst->getAllocatedType();
    std::string name = (allocaInst->getName());

    PointerOperation *allocOp;

    if (auto ptrType = llvm::dyn_cast<llvm::PointerType>(type)) {

        allocOp = new PointerAllocationOp(name, PointerFormat(ptrType), allocaInst);

    } else if (type->isStructTy() || type->isArrayTy()) {

        auto compType = llvm::cast<llvm::CompositeType>(type);
        allocOp = new CompositeAllocationOp(compType, dataLayout, name, allocaInst);

    } else {

        int size = dataLayout->getTypeAllocSize(type);

        allocOp = new VarAllocationOp(name, size, allocaInst);

    }

    return allocOp;
}

/**
 * handles copy instructions. Throws PointerIrrelevantException.
 * @param loadInstructions the load instruction of the copy
 * @param storeInst the store instruction of the copy
 * @return the resulting Copy Pointer Operation
 */
PointerOperation *AbstractFunctionFactory::handleStore(llvm::StoreInst *storeInst) {

    llvm::Value *fromValue = storeInst->getValueOperand();
    llvm::Value *toValue = storeInst->getPointerOperand();

    if (!fromValue->getType()->isPointerTy()) {
        throw PointerIrrelevantException();
    }


    PointerFinder *fromFinder = nullptr;
    PointerFinder *toFinder = nullptr;


    fromFinder = finderFactory->getPointerFinder(fromValue, false);
    toFinder = finderFactory->getPointerFinder(toValue, true);

    assert(fromFinder != nullptr);
    assert(toFinder != nullptr);

    StoreOp *copyOp = new StoreOp(fromFinder, toFinder, storeInst);

    return copyOp;
}

//PointerOperation *AbstractFunctionFactory::handleAssignment(llvm::StoreInst *storeInst) {
//
//    llvm::Value *targetValue = storeInst->getValueOperand();
//    llvm::Value *pointerValue = storeInst->getPointerOperand();
//
//
//    if (!targetValue->getType()->isPointerTy()) { throw PointerIrrelevantException(); }
//
//    auto ptrType = llvm::cast<llvm::PointerType>(pointerValue->getType());
//    if (!ptrType->getElementType()->isPointerTy()) { throw PointerIrrelevantException(); }
//
//
//    PointerFinder *pointerFinder;
//    TargetFinder *targetFinder;
//    std::list<llvm::Instruction *> assocInsts;
//
//
//    targetFinder = finderFactory->getTargetFinder(targetValue);
//    pointerFinder = finderFactory->getPointerFinder(pointerValue, false);
//
//    assert(pointerFinder != nullptr);
//    assert(targetFinder != nullptr);
//
//    AssignmentOp *assignmentOp = new AssignmentOp(pointerFinder, targetFinder, storeInst,
//                                                  assocInsts);
//
//    return assignmentOp;
//}

PointerOperation *AbstractFunctionFactory::handleCallWithIrrelevantReturn(llvm::CallInst *callInst) {

    llvm::Function *function = callInst->getCalledFunction();
    std::string funcName = function->getName();

    std::map<int, PointerFinder *> pointerParamFinders;
    std::map<int, CompositeFinder *> compositeParamFinders;

    for (int i = 0; i < function->getNumOperands(); ++i) {

        llvm::outs() << "i = " << i << ", getNumOperands() = " << function->getNumOperands() << "\n";

        llvm::Value *param = callInst->getArgOperand(i);

        if (param->getType()->isPointerTy()) {

            pointerParamFinders[i] = finderFactory->getPointerFinder(param, false);

        } else if (param->getType()->isStructTy() || param->getType()->isArrayTy()) {

            compositeParamFinders[i] = finderFactory->getCompositeFinder(param);
        }
    }

    FunctionFinder *funcFinder = finderFactory->getFunctionFinder(function);

    return new CallWithIrrelevantReturnOp(funcFinder, callInst, pointerParamFinders, compositeParamFinders);

}

ReturnOp *AbstractFunctionFactory::handleReturn(llvm::ReturnInst *returnInst) {

    llvm::Value *returnValue = returnInst->getReturnValue();

    if (returnValue == nullptr) return new ReturnNilOp(returnInst);
    // TODO change if predicate when functions can return composites
    if (!returnValue->getType()->isPointerTy()) return new ReturnNilOp(returnInst);

    llvm::outs() << "Return value " << returnValue->getName() << "\n";

    PointerFinder *returnFinder = finderFactory->getPointerFinder(returnValue, false);

    return new ReturnPointerOp(returnFinder, returnInst);


}

ReturnOp * AbstractFunctionFactory::buildFinalOp(llvm::ReturnInst *returnInst) {

    if (finalOp != nullptr) throw MultipleReturnException();

    ReturnOp *op = handleReturn(returnInst);

    assert(op != nullptr);

    return op;

}


