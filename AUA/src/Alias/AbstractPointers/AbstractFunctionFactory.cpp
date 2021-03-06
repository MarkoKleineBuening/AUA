//
// Created by mlaupichler on 10.07.19.
//

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
#include "AUA/Alias/AbstractPointers/PointerIrrelevantException.h"

AbstractFunctionFactory::AbstractFunctionFactory(llvm::DataLayout *dataLayout, FinderFactory *finderFactory)
        : dataLayout(dataLayout), finderFactory(finderFactory) {}


AbstractFunction *AbstractFunctionFactory::buildAbstractFunction(llvm::Function *function) {

    llvm::outs() << "Abstracting function " << function->getName() << ":\n";

    totalInstructionCount += function->getInstructionCount();

    auto state = new std::map<llvm::BasicBlock*, BlockAbstractionState*>();

    std::string name = function->getName();

    auto initialOp = new DummyInitialOp();

    llvm::BasicBlock* entryBlock = &function->getEntryBlock();

    auto topLevelLoopBlockInfo = getTopLevelLoopBlockInfo(function);

    abstractBasicBlock(entryBlock, state, topLevelLoopBlockInfo);

    initialOp->linkSuccessor(state->at(entryBlock)->firstReachable);

    std::map<int, PointerFormat>* pointerParamFormats = getPointerParamFormats(function);
    std::map<int, CompositeFormat>* compositeParamFormats = getCompositeParamFormats(function);
    std::set<AbstractVar*>* varParams = getVarParams(function);
    std::vector<std::string> paramNames = getParamNames(function);


    auto resultFunc = new AbstractFunction(initialOp, state->at(entryBlock)->lastReachable, name, pointerParamFormats,
                                           compositeParamFormats, varParams, paramNames);

    return resultFunc;

}

LoopBlockInfo* AbstractFunctionFactory::getTopLevelLoopBlockInfo(llvm::Function *function) {

    auto loopBlockInfo = new LoopBlockInfo();

    auto DT = new llvm::DominatorTree();
    DT->recalculate(*function);

    auto KLoop = new llvm::LoopInfoBase<llvm::BasicBlock, llvm::Loop>();
    KLoop->releaseMemory();
    KLoop->analyze(*DT);

    for (llvm::Loop *L : *KLoop) {

        llvm::outs() << L->getName() << "\n";
        auto abstractL = new AbstractLoop(L);
        loopBlockInfo->loopsByHeader[abstractL->header] = abstractL;
    }

    return loopBlockInfo;
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

void AbstractFunctionFactory::abstractBasicBlock(llvm::BasicBlock *BB,
                                                 std::map<llvm::BasicBlock *, BlockAbstractionState *> *state,
                                                 LoopBlockInfo *loopBlockInfo) {

    if (state->find(BB) == state->end()) {

        state->emplace(BB, new BlockAbstractionState());
    }

    if (state->at(BB)->done) return;

    auto opRange = abstractBlockInstructions(BB);
    PointerOperation* first = (opRange == nullptr)? nullptr : opRange->first;
    PointerOperation* last = (opRange == nullptr)? nullptr : opRange->second;

    if (loopBlockInfo->loopsByHeader.find(BB) != loopBlockInfo->loopsByHeader.end()) {

        llvm::outs() << "Block " << BB->getName() << " is loop header.\n";

        // Block is loop header => abstract exit first, then prepare latch info and then abstract loop itself

        auto loop = loopBlockInfo->loopsByHeader[BB];

        auto exitBlock = loop->exit;
        abstractBasicBlock(exitBlock, state, loopBlockInfo);

        auto lastFromExit = state->at(exitBlock)->lastReachable;

        auto joinOp = new JoinOp();
        auto splitOp = new SplitOp();

        splitOp->linkSuccessor(joinOp);

        auto subLoops = loop->getSubAbstractLoopsByHeader();
        loopBlockInfo->loopsByHeader.merge(subLoops);

        for (auto latch : loop->latches) {

            loopBlockInfo->loopLatchData.emplace(latch, new LoopLatchInfo(joinOp, lastFromExit));
        }


        abstractBasicBlock(loop->entry, state, loopBlockInfo);

        splitOp->linkSuccessor(state->at(loop->entry)->firstReachable);

        if (first == nullptr) {
            assert(last == nullptr);

            first = splitOp;
            last = joinOp;
        } else {
            assert(last != nullptr);

            last->linkSuccessor(splitOp);
            last = joinOp;
        }

        last->linkSuccessor(state->at(exitBlock)->firstReachable);

        state->at(BB)->firstReachable = first;
        state->at(BB)->lastReachable = lastFromExit;
        state->at(BB)->done = true;

        return;
    }

    llvm::outs() << "Block " << BB->getName() << " is not loop header.\n";

    if (loopBlockInfo->loopLatchData.find(BB) != loopBlockInfo->loopLatchData.end()) {

        llvm::outs() << "Block " << BB->getName() << " is loop latch.\n";

        // BB is loop latch => attach it to the join op of the header and take its last reachable as specified in loopBlockInfo->loopLatchData

        if (first == nullptr) {
            assert(last == nullptr);

            state->at(BB)->firstReachable = loopBlockInfo->loopLatchData[BB]->headerJoin;
            state->at(BB)->lastReachable = loopBlockInfo->loopLatchData[BB]->headerLastReachable;

            state->at(BB)->done = true;
            return;
        }

        assert ((first != nullptr) && (last != nullptr));

        last->linkSuccessor(loopBlockInfo->loopLatchData[BB]->headerJoin);
        state->at(BB)->firstReachable = first;
        state->at(BB)->lastReachable = loopBlockInfo->loopLatchData[BB]->headerLastReachable;

        state->at(BB)->done = true;
        return;

    }

    llvm::outs() << "Block " << BB->getName() << " is not loop latch.\n";

    int numBlockPreds = std::distance(llvm::pred_begin(BB), llvm::pred_end(BB));

    if (numBlockPreds > 1) {

        auto joinOp = new JoinOp();

        if (first == nullptr) {
            assert(last == nullptr);
            first = joinOp;
            last = joinOp;
        } else {
            assert(last != nullptr);

            joinOp->linkSuccessor(first);
            first = joinOp;
        }

    }



    int numBlockSuccs = std::distance(llvm::succ_begin(BB), llvm::succ_end(BB));

    if (numBlockSuccs == 0) {

        auto terminator = BB->getTerminator();
        auto returnInst = llvm::cast<llvm::ReturnInst>(terminator);

        auto returnOp = handleReturn(returnInst);
        assert(returnOp != nullptr);

        if (first == nullptr) {
            assert(last == nullptr);

            state->at(BB)->firstReachable = returnOp;
        } else {
            assert(last != nullptr);

            state->at(BB)->firstReachable = first;
            last->linkSuccessor(returnOp);
        }

        last = returnOp;

        state->at(BB)->lastReachable = returnOp;
        state->at(BB)->done = true;

        return;

    }

    if (numBlockSuccs > 1) {

        auto splitOp = new SplitOp();

        if (first == nullptr) {
            assert(last == nullptr);
            first = splitOp;
            last = splitOp;
        } else {
            assert(last != nullptr);

            last->linkSuccessor(splitOp);
            last = splitOp;
        }

        ReturnOp* lastReachableOfSuccs = nullptr;
        for (auto successor : llvm::successors(BB)) {

            abstractBasicBlock(successor, state, loopBlockInfo);
            last->linkSuccessor(state->at(successor)->firstReachable);

            if (lastReachableOfSuccs == nullptr) {
                lastReachableOfSuccs = state->at(successor)->lastReachable;
            } else if (state->at(successor)->lastReachable != lastReachableOfSuccs) {
                throw MultipleReturnException();
            }
        }

        state->at(BB)->firstReachable = first;
        state->at(BB)->lastReachable = lastReachableOfSuccs;
        state->at(BB)->done = true;
        return;

    }

    assert(numBlockSuccs == 1);

    auto terminator = BB->getTerminator();
    assert(terminator->getNumSuccessors() == 1);

    llvm::BasicBlock* successor = terminator->getSuccessor(0);

    abstractBasicBlock(successor, state, loopBlockInfo);

    auto firstReachableOfSucc = state->at(successor)->firstReachable;
    auto lastReachableOfSucc = state->at(successor)->lastReachable;

    if (first == nullptr) {
        assert(last == nullptr);

        state->at(BB)->firstReachable = firstReachableOfSucc;
        state->at(BB)->lastReachable = lastReachableOfSucc;
    } else {
        assert(last != nullptr);

        state->at(BB)->firstReachable = first;

        last->linkSuccessor(firstReachableOfSucc);
        state->at(BB)->lastReachable = lastReachableOfSucc;
    }

    state->at(BB)->done = true;
}


/**
 * Abstracts all pointer relevant instructions from BB into PointerOperations. It ignores all control flow instructions.
 * @param BB the BasicBlock whose instructions to abstract.
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

    if (auto storeInst = llvm::dyn_cast<llvm::StoreInst>(inst)) {

        return handleStore(storeInst);

    } else if (auto callInst = llvm::dyn_cast<llvm::CallInst>(inst)) {

        totalCallCount += 1;

        if (callInst->isIndirectCall()) {

            // Ignore indirect calls (calls via function pointer)

            throw PointerIrrelevantException();
        }


        auto calledFunction = callInst->getCalledFunction();

        if (calledFunction == nullptr) {
            // Ignore indirect calls that haven't been caught above
            throw PointerIrrelevantException();
        }

        if (calledFunction->isIntrinsic()) {

            throw PointerIrrelevantException();
        }

        if (!Configuration::global->hasGlobalFunction(calledFunction->getName())) {

            Configuration::global->addGlobalFunction(buildAbstractFunction(calledFunction));
        }

        // TODO Change predicate once composite returns are okay
        if ((!callInst->getFunctionType()->getReturnType()->isPointerTy()) || (!callInst->hasNUsesOrMore(1))) {


            return handleCallWithIrrelevantReturn(callInst);
        }

    } else if (auto allocaInst = llvm::dyn_cast<llvm::AllocaInst>(inst)) {

        return handleAllocation(allocaInst);
    }

    throw PointerIrrelevantException();

}

int countPointerMembersRecursively(llvm::CompositeType* compType) {

    int pointerMemberCount = 0;

    int memberCount;
    if (llvm::ArrayType *arrayType = llvm::dyn_cast<llvm::ArrayType>(compType)) {
        memberCount = arrayType->getNumElements();
    } else if (llvm::StructType *structType = llvm::dyn_cast<llvm::StructType>(compType)) {
        memberCount = structType->getNumElements();
    } else {
        throw NoCompositeTypeException();
    }

    for (int i = 0; i < memberCount; ++i) {

        auto memberType = compType->getTypeAtIndex(i);

        if (memberType->isPointerTy()) {

            pointerMemberCount += 1;

        } else if (memberType->isStructTy() || memberType->isArrayTy()) {

            auto compType = llvm::cast<llvm::CompositeType>(memberType);
            pointerMemberCount += countPointerMembersRecursively(compType);
        }
    }

    return pointerMemberCount;

}

PointerOperation *AbstractFunctionFactory::handleAllocation(llvm::AllocaInst *allocaInst) {

    llvm::Type *type = allocaInst->getAllocatedType();
    std::string name = (allocaInst->getName());

    PointerOperation *allocOp;

    if (auto ptrType = llvm::dyn_cast<llvm::PointerType>(type)) {

        allocOp = new PointerAllocationOp(name, PointerFormat(ptrType), allocaInst);

        totalPointerCount += 1;

    } else if (type->isStructTy() || type->isArrayTy()) {

        llvm::outs() << type->getTypeID() << "\n";
        auto compType = llvm::cast<llvm::CompositeType>(type);
        allocOp = new CompositeAllocationOp(compType, dataLayout, name, allocaInst);

        totalPointerCount += countPointerMembersRecursively(compType);

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

    auto copyOp = new StoreOp(fromFinder, toFinder, storeInst);

    return copyOp;
}

PointerOperation *AbstractFunctionFactory::handleCallWithIrrelevantReturn(llvm::CallInst *callInst) {


    llvm::Function *function = callInst->getCalledFunction();

    std::string funcName = function->getName();

    llvm::outs() << "Handling non pointer return function call to " << funcName << ".\n";

    std::map<int, PointerFinder *> pointerParamFinders;
    std::map<int, CompositeFinder *> compositeParamFinders;

    for (int i = 0; i < function->arg_size(); ++i) {


        llvm::Value *param = callInst->getArgOperand(i);


        if (param->getType()->isPointerTy()) {


            pointerParamFinders[i] = finderFactory->getPointerFinder(param, false);

        } else if (param->getType()->isStructTy() || param->getType()->isArrayTy()) {


            compositeParamFinders[i] = finderFactory->getCompositeFinder(param);
        }
    }

    FunctionFinder *funcFinder = finderFactory->getFunctionFinder(function);

    auto op = new CallWithIrrelevantReturnOp(funcFinder, callInst, pointerParamFinders, compositeParamFinders);

    llvm::outs() << "Finished building non pointer return function call to " << funcName << ".\n";

    return op;
}

ReturnOp *AbstractFunctionFactory::handleReturn(llvm::ReturnInst *returnInst) {

    llvm::Value *returnValue = returnInst->getReturnValue();

    if (returnValue == nullptr) return new ReturnNilOp(returnInst);
    // TODO change if predicate when functions can return composites
    if (!returnValue->getType()->isPointerTy()) return new ReturnNilOp(returnInst);

    if (llvm::isa<llvm::ConstantPointerNull>(returnValue)) return new ReturnNilOp(returnInst);

    llvm::outs() << "Return value " << returnValue->getName() << "\n";

    PointerFinder *returnFinder = finderFactory->getPointerFinder(returnValue, false);

    return new ReturnPointerOp(returnFinder, returnInst);


}


