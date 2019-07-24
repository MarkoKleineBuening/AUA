#include <utility>

//
// Created by mlaupichler on 27.05.19.
//

#include "AUA/Alias/AbstractOps/CopyOp.h"
#include <llvm/Support/FileSystem.h>
#include <llvm/IR/Instructions.h>

CopyOp::CopyOp(PointerFinder *fromFinder, PointerFinder *toFinder, llvm::StoreInst *storeInst,
               std::list<llvm::LoadInst *> loadInsts, std::list<llvm::Instruction *> assocInsts)
        : fromFinder(fromFinder), toFinder(toFinder), storeInstruction(storeInst),
          loadInstructions(std::move(loadInsts)),
          assocInsts(std::move(assocInsts)) {}

Configuration *CopyOp::apply(Configuration *in) {

    assert(fromFinder != nullptr);
    assert(toFinder != nullptr);

    auto fromPointers = fromFinder->findPointers(in);
    auto toPointers = toFinder->findPointers(in);

    assert(fromPointers->getFormat() == toPointers->getFormat());

    std::set<AbstractTarget> allTargetsOfFrom = fromPointers->getMergedTargets();

    if (toPointers->size() == 1) {

        AbstractPointer *toPointer = *(toPointers->asSet().begin());
        toPointer->setTargets(allTargetsOfFrom);

    } else {

        for (auto toPointer : toPointers->asSet()) {

            toPointer->addTargets(allTargetsOfFrom);
        }
    }


//    std::set<llvm::Instruction*>* allAssocInsts = new std::set<llvm::Instruction*>();
//
//    allAssocInsts->insert(assocInsts.begin(), assocInsts.end());
//    allAssocInsts->insert(loadInstructions.begin(), loadInstructions.end());
//    allAssocInsts->insert((llvm::Instruction*) storeInstruction);

    return in;

}

std::set<llvm::Instruction *> CopyOp::getAssocInstructions() {

    std::set<llvm::Instruction *> result;
    result.insert(assocInsts.begin(), assocInsts.end());
    result.insert(loadInstructions.begin(), loadInstructions.end());
    result.insert((llvm::Instruction *) storeInstruction);

    return result;

}
