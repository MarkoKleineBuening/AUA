//
// Created by mlaupichler on 27.05.19.
//

#include "AUA/Alias/AbstractOps/CopyOp.h"
#include <llvm/Support/FileSystem.h>
#include <llvm/IR/Instructions.h>

CopyOp::CopyOp(PointerFinder *fromFinder, PointerFinder *toFinder, int derefDepth, llvm::StoreInst *storeInst,
               const std::list<llvm::LoadInst *> loadInsts,
               const std::list<llvm::GetElementPtrInst *> gepInstructions)
        : fromFinder(fromFinder), toFinder(toFinder), derefDepth(derefDepth), storeInstruction(storeInst), loadInstructions(loadInsts),
          gepInstructions(gepInstructions) {}

Configuration* CopyOp::apply(Configuration* in) {

    assert(fromFinder != nullptr);
    assert(toFinder != nullptr);

    auto from = fromFinder->findPointer(in);
    auto to = toFinder->findPointer(in);

    std::set<llvm::Instruction*> allAssocInsts = from->getAssocInsts();


    assert(from->getPointerLevel() == to->getPointerLevel() + derefDepth);

    std::set<AbstractTarget> upperTargets = from->getTargets();

    for (int i = 0; i < derefDepth; ++i) {

        std::set<AbstractTarget> lowerTargets;

        for (AbstractTarget target : upperTargets) {

            assert(target.base->getPointerLevel() > 0 && target.base->getPointerLevel() == from->getPointerLevel() - (i + 1));

            //technically unsafe. Covered by assertion above
            AbstractPointer* lowerPointer = in->pointers[target.base->getName()];

            assert(lowerPointer != nullptr);

            allAssocInsts.merge(lowerPointer->getAssocInsts());
            lowerTargets.merge(lowerPointer->getTargets());
        }

        upperTargets = lowerTargets;

    }

    to->setTargets(upperTargets);

    allAssocInsts.insert(gepInstructions.begin(), gepInstructions.end());
    allAssocInsts.insert(loadInstructions.begin(), loadInstructions.end());
    allAssocInsts.insert((llvm::Instruction*) storeInstruction);

    to->setAssocInsts(allAssocInsts);

    return in;

}

std::set<llvm::Instruction *> CopyOp::getAssocInstructions() {

    std::set<llvm::Instruction *> result;
    result.insert(gepInstructions.begin(), gepInstructions.end());
    result.insert(loadInstructions.begin(), loadInstructions.end());
    result.insert((llvm::Instruction*) storeInstruction);

    return result;

}
