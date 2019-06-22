#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <set>
#include <list>

#include <llvm/Support/FileSystem.h>
#include <llvm/Support/MemoryBuffer.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/Bitcode/BitcodeReader.h>
#include <llvm/IR/InstIterator.h>
#include <llvm/IR/Instruction.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/InstrTypes.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/DataLayout.h>
#include <llvm/ADT/Optional.h>
#include <llvm/ADT/StringRef.h>
#include <llvm/IR/CFG.h>

#include <llvm/Analysis/LoopInfo.h>

#include "PointerIrrelevantException.h"

#include <AUA/Alias/AbstractPointers/AbstractPointer.h>
#include <AUA/Alias/AbstractOps/AssignmentOp.h>
#include <AUA/Alias/AbstractPointers/Configuration.h>
#include <AUA/Alias/AbstractOps/PointerAllocationOp.h>
#include <AUA/Alias/AbstractOps/VarAllocationOp.h>
#include <AUA/Alias/AbstractOps/CopyOp.h>
#include <AUA/Alias/AbstractOps/DummyFinalOp.h>
#include <AUA/Alias/AbstractOps/JoinOp.h>
#include <AUA/Alias/AbstractOps/SplitOp.h>
#include <AUA/Alias/AbstractOps/DummyInitialOp.h>
#include <AUA/Alias/AbstractOps/CompositeAllocationOp.h>
#include <AUA/Alias/AbstractPointers/Finders/MemberPointerFinder.h>
#include <AUA/Alias/AbstractPointers/Finders/NonMemberPointerFinder.h>
#include <AUA/Alias/AbstractPointers/Finders/MemberTargetFinder.h>
#include <AUA/Alias/AbstractPointers/Finders/NonMemberTargetFinder.h>


llvm::DataLayout* dataLayout;

/**
 * Used to start the chain of PointerOperations. Is the only PointerOperation allowed to have no predecessors.
 */
DummyInitialOp* initialOp = new DummyInitialOp();

/**
 * Used to collect dangling paths of PointerOperations into one final PointerOperation. Is the only PointerOperation allowed to have no successors.
 */
DummyFinalOp* finalOp = new DummyFinalOp();


std::map<std::string, bool> loopLatchFound;
std::map<std::string, JoinOp*> loopJoinOps;

std::map<std::string, JoinOp*> ifJoinOps;



std::unique_ptr<llvm::Module> readInModule(llvm::LLVMContext &context, std::string inFile);
PointerOperation *abstractBasicBlock(llvm::BasicBlock *BB);
PointerOperation *abstractLoopConditionBlock(llvm::BasicBlock *BB);
PointerOperation *abstractIfRejoinBlock(llvm::BasicBlock *BB);

PointerOperation* abstractInstruction(llvm::Instruction *I);
PointerOperation* handleAllocation(llvm::AllocaInst *allocaInst);
PointerOperation *handleCopy(llvm::StoreInst *storeInst);
PointerOperation* handleAssignment(llvm::StoreInst *storeInst);
int getMemberIdx(llvm::GetElementPtrInst* gepInst);

PointerOperation *abstractIfBranchBlock(llvm::BasicBlock *BB);

std::pair<PointerOperation *, PointerOperation *> * abstractBlockInstructions(llvm::BasicBlock *BB);

int main(int argc, char **argv) {

    std::string inFile = argv[1];
    llvm::LLVMContext context;
    llvm::Module *module = readInModule(context,inFile).release();

    dataLayout = new llvm::DataLayout(module);

    for (auto &F: module->getFunctionList()) {


        PointerOperation* firstOp = abstractBasicBlock(&F.getEntryBlock());
        initialOp->linkSuccessor(firstOp);
    }


    Configuration* configuration = new Configuration();

    initialOp->execute(configuration);

    return 0;
}



/**
 * Abstracts a given BasicBlock into an ordered graph of PointerOperations and attaches it to the given preceeding PointerOperation.
 * Recursively calls abstractBasicBlock for all succeeding BasicBlocks to complete the ordered graph.
 * @param BB the BasicBlock to abstract
 * @param predExitOp the last preceeding PointerOperation
 * @return The first PointerOperation of this block if any are abstracted from it or the result of its following block else.
 */
PointerOperation *abstractBasicBlock(llvm::BasicBlock *BB) {

    llvm::outs() << "\nAbstracting block " << BB->getName() << "\n";


    // Block is loop condition iff it has two predecessors and two successors
    if(BB->hasNPredecessors(2) && (BB->getTerminator()->getNumSuccessors() == 2)) {

        return abstractLoopConditionBlock(BB);


    }

    //Block is if terminated iff it has less than two predecessors and two successors
    if(!BB->hasNPredecessorsOrMore(2) && (BB->getTerminator()->getNumSuccessors() == 2)) {

        return abstractIfBranchBlock(BB);


    }

    //Block is if rejoin iff it has two predecessors and less than two successors
    if(BB->hasNPredecessors(2) && (BB->getTerminator()->getNumSuccessors() < 2)) {

        return abstractIfRejoinBlock(BB);


    }

    PointerOperation* first = nullptr;
    PointerOperation* last = nullptr;

    auto abstractedOpPair = abstractBlockInstructions(BB);
    if (abstractedOpPair != nullptr) {

        first = abstractedOpPair->first;
        last = abstractedOpPair->second;
    }

    PointerOperation* succOp;

    if (BB->getTerminator()->getNumSuccessors() == 0) {

        succOp = finalOp;

    } else if (BB->getTerminator()->getNumSuccessors() == 1) {

        succOp = abstractBasicBlock(BB->getTerminator()->getSuccessor(0));

    } else {
        throw new std::exception();
    }


    if (last == nullptr) {
        return succOp;
    }

    last->linkSuccessor(succOp);
    return first;

}

PointerOperation *abstractLoopConditionBlock(llvm::BasicBlock *BB) {

    assert(BB->hasNPredecessors(2));
    assert(BB->getTerminator()->getNumSuccessors() == 2);

    if (loopJoinOps.find(BB->getName()) != loopJoinOps.end()) {

        loopLatchFound[BB->getName()] = true;
        return loopJoinOps[BB->getName()];

    }

    loopLatchFound[BB->getName()] = false;

    PointerOperation* first = nullptr;
    PointerOperation* last = nullptr;

    auto abstractedOpPair = abstractBlockInstructions(BB);
    if (abstractedOpPair != nullptr) {

        first = abstractedOpPair->first;
        last = abstractedOpPair->second;
    }


    SplitOp* splitOp = new SplitOp();
    JoinOp* joinOp = new JoinOp();
    splitOp->linkSuccessor(joinOp);
    loopJoinOps[BB->getName()] = joinOp;

    if (first == nullptr) {
        first = splitOp;
    } else {
        last->linkSuccessor(splitOp);
    }

    PointerOperation* loopBodySuccOp;
    PointerOperation* loopExitSuccOp;

    PointerOperation* firstSuccOp = abstractBasicBlock(BB->getTerminator()->getSuccessor(0));

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

PointerOperation *abstractIfBranchBlock(llvm::BasicBlock *BB) {

    assert(!BB->hasNPredecessorsOrMore(2));
    assert(BB->getTerminator()->getNumSuccessors() == 2);

    PointerOperation* first = nullptr;
    PointerOperation* last = nullptr;

    auto abstractedOpPair = abstractBlockInstructions(BB);
    if (abstractedOpPair != nullptr) {

        first = abstractedOpPair->first;
        last = abstractedOpPair->second;
    }

    SplitOp* splitOp = new SplitOp();
    if (first == nullptr) {
        first = splitOp;
    } else {
        last->linkSuccessor(splitOp);
    }

    PointerOperation* firstSuccOp = abstractBasicBlock(BB->getTerminator()->getSuccessor(0));
    splitOp->linkSuccessor(firstSuccOp);
    PointerOperation* secondSuccOp = abstractBasicBlock(BB->getTerminator()->getSuccessor(1));
    splitOp->linkSuccessor(secondSuccOp);

    return first;

}

PointerOperation *abstractIfRejoinBlock(llvm::BasicBlock *BB) {

    assert(BB->hasNPredecessors(2));
    assert(BB->getTerminator()->getNumSuccessors() < 2);

    if (ifJoinOps.find(BB->getName()) != ifJoinOps.end()) {

        return ifJoinOps[BB->getName()];
    }

    PointerOperation* first = nullptr;
    PointerOperation* last = nullptr;

    auto abstractedOpPair = abstractBlockInstructions(BB);
    if (abstractedOpPair != nullptr) {

        first = abstractedOpPair->first;
        last = abstractedOpPair->second;
    }

    JoinOp* joinOp = new JoinOp();
    ifJoinOps[BB->getName()] = joinOp;

    if(first == nullptr) {
        last = joinOp;
    } else {
        joinOp->linkSuccessor(first);
    }

    PointerOperation* succOp;

    if (BB->getTerminator()->getNumSuccessors() == 0) {

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
std::pair<PointerOperation *, PointerOperation *> * abstractBlockInstructions(llvm::BasicBlock *BB) {


    PointerOperation* first = nullptr;
    PointerOperation* last = nullptr;


    for (llvm::BasicBlock::iterator BBI = BB->begin(), BBE = BB->end();
         BBI != BBE;
         ++BBI) {

        llvm::Instruction *inst = &*BBI;

        const char *termMsg = (inst->isTerminator()) ? " (Terminator)" : "";
        llvm::outs() << *inst << termMsg << "\n";

        try {

            PointerOperation* op = abstractInstruction(inst);

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
        return new std::pair<PointerOperation*, PointerOperation*>(first, last);
    }

    llvm::outs() << "Block " << BB->getName() << " has no PointerOperations.\n";
    return nullptr;

}

PointerOperation* abstractInstruction(llvm::Instruction *inst) {

    if (llvm::StoreInst *storeInst = llvm::dyn_cast<llvm::StoreInst>(inst)) {

        llvm::Value* fromValue = storeInst->getValueOperand();

        if(llvm::isa<llvm::LoadInst>(fromValue)) {
            return handleCopy(storeInst);
        } else {
            return handleAssignment(storeInst);
        }

    }

    if(llvm::AllocaInst *allocaInst = llvm::dyn_cast<llvm::AllocaInst>(inst)) {
        return handleAllocation(allocaInst);
    }

    throw PointerIrrelevantException();

}

PointerOperation* handleAllocation(llvm::AllocaInst *allocaInst) {

    llvm::Type *type = allocaInst->getAllocatedType();
    std::string name = (allocaInst->getName());

    PointerOperation* allocOp;

    if (type->isPointerTy()) {

        int ptrLevel = 0;

        while(type->isPointerTy()) {

            llvm::PointerType* ptrType = llvm::dyn_cast<llvm::PointerType>(type);
            assert(ptrType != nullptr);
            type = ptrType->getElementType();
            ++ptrLevel;
        }

        allocOp = new PointerAllocationOp(name, ptrLevel, allocaInst);

    } else if (type->isStructTy() || type->isArrayTy()) {

        llvm::CompositeType* compType = llvm::cast<llvm::CompositeType>(type);
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
PointerOperation *handleCopy(llvm::StoreInst *storeInst) {

    std::list<llvm::LoadInst*> loadInstructions;
    llvm::Value* fromValue = storeInst->getValueOperand();

    while(llvm::LoadInst* loadInst = llvm::dyn_cast<llvm::LoadInst>(fromValue)) {

        loadInstructions.push_front(loadInst);
        fromValue = loadInst->getPointerOperand();

    }

    int derefDepth = loadInstructions.size() - 1;

    llvm::Value* toValue = storeInst->getPointerOperand();

    PointerFinder *fromFinder = nullptr;
    PointerFinder *toFinder = nullptr;

    if (llvm::GetElementPtrInst* fromGEPInst = llvm::dyn_cast<llvm::GetElementPtrInst>(fromValue)) {

        std::string fromCompName = fromGEPInst->getPointerOperand()->getName();
        int fromMemberIdx = getMemberIdx(fromGEPInst);

        fromFinder = new MemberPointerFinder(fromCompName, fromMemberIdx);
    } else {
        fromFinder = new NonMemberPointerFinder(fromValue->getName());
    }

    if (llvm::GetElementPtrInst* toGEPInst = llvm::dyn_cast<llvm::GetElementPtrInst>(toValue)) {

        std::string toCompName = toGEPInst->getPointerOperand()->getName();
        int toMemberIdx = getMemberIdx(toGEPInst);

        toFinder = new MemberPointerFinder(toCompName, toMemberIdx);
    } else {
        toFinder = new NonMemberPointerFinder(toValue->getName());
    }

    assert(fromFinder != nullptr);
    assert(toFinder != nullptr);

    CopyOp* copyOp = new CopyOp(fromFinder, toFinder, derefDepth, storeInst, loadInstructions);

    return copyOp;
}

int getMemberIdx(llvm::GetElementPtrInst* gepInst) {

    int opCount = gepInst->getNumOperands();
    llvm::ConstantInt *CI = llvm::cast<llvm::ConstantInt>(gepInst->getOperand(opCount - 1));
    assert(CI != nullptr);
    int memberIdx = CI->getZExtValue();

    return memberIdx;

}

PointerOperation* handleAssignment(llvm::StoreInst *storeInst) {

    llvm::Value* targetValue = storeInst->getValueOperand();
    llvm::Value* pointerValue = storeInst->getPointerOperand();


    if(!targetValue->getType()->isPointerTy()) {throw PointerIrrelevantException();}

    llvm::PointerType* ptrType = llvm::cast<llvm::PointerType>(pointerValue->getType());
    if(!ptrType->getElementType()->isPointerTy()){throw PointerIrrelevantException();}


    PointerFinder* pointerFinder;
    TargetFinder* targetFinder;

    if (llvm::GetElementPtrInst* pointerGEPInst = llvm::dyn_cast<llvm::GetElementPtrInst>(pointerValue)) {

        std::string pointerCompName = pointerGEPInst->getPointerOperand()->getName();
        int pointerMemberIdx = getMemberIdx(pointerGEPInst);

        pointerFinder = new MemberPointerFinder(pointerCompName, pointerMemberIdx);
    } else {
        pointerFinder = new NonMemberPointerFinder(pointerValue->getName());
    }

    if(llvm::GetElementPtrInst* targetGEPInst = llvm::dyn_cast<llvm::GetElementPtrInst>(targetValue)) {

        std::string targetCompName = targetGEPInst->getPointerOperand()->getName();
        int targetMemberIdx = getMemberIdx(targetGEPInst);

        targetFinder = new MemberTargetFinder(targetCompName, targetMemberIdx);
    } else {
        targetFinder = new NonMemberTargetFinder(targetValue->getName());
    }

    assert(pointerFinder != nullptr);
    assert(targetFinder != nullptr);

    AssignmentOp* assignmentOp = new AssignmentOp(pointerFinder, targetFinder, storeInst);

    return assignmentOp;
}

/**
* creates llvm module based on llvm context and the input file that was given while constructing the compile object
* @param llvmContext
* @return
*/
std::unique_ptr<llvm::Module> readInModule(llvm::LLVMContext &context, std::string inputFile) {
    std::cout << "getting module from bitcode for file " << inputFile << "\n";
    llvm::ErrorOr <std::unique_ptr<llvm::MemoryBuffer>> bufferOrError =
            llvm::MemoryBuffer::getFile(inputFile);
    if (std::error_code EC = bufferOrError.getError()) {
        std::cout  << "Cannot open bitcode file (errorCode " + EC.message() + ")";
    }
    std::unique_ptr <llvm::MemoryBuffer> buffer(std::move(*bufferOrError));
    llvm::MemoryBufferRef bitcodeBufferRef(buffer->getBuffer(), buffer->getBufferIdentifier());
    llvm::Expected <std::unique_ptr<llvm::Module>> expectedBcForSlicing =
            llvm::parseBitcodeFile(bitcodeBufferRef, context);
    if (!expectedBcForSlicing) {
        std::error_code ec = llvm::errorToErrorCode(expectedBcForSlicing.takeError());
        std::cout << "error reading bitcode from buffer ";
    }
    return std::move(expectedBcForSlicing.get());
}