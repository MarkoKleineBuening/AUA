//
// Created by mlaupichler on 27.05.19.
//

#include "AUA/Alias/AbstractOps/CopyOp.h"
#include <llvm/Support/FileSystem.h>
#include <llvm/IR/Instructions.h>

CopyOp::CopyOp(std::string fromName, std::string toName, int derefDepth, llvm::StoreInst *storeInst,
               const std::list<llvm::LoadInst*> loadInsts) : fromName(fromName), toName(toName), derefDepth(derefDepth), storeInstruction(storeInst), loadInstructions(loadInsts) {}

Configuration* CopyOp::apply(Configuration* in) {


    auto from = in->pointers[fromName];
    auto to = in->pointers[toName];

    std::set<llvm::Instruction*> allAssocInsts = from->getAssocInsts();


    assert(from->getLevel() == to->getLevel() + derefDepth);

    std::set<AbstractTarget> upperTargets = from->getTargets();

    for (int i = 0; i < derefDepth; ++i) {

        std::set<AbstractTarget> lowerTargets;

        for (AbstractTarget target : upperTargets) {

            assert(target.base->getLevel() > 0 && target.base->getLevel() == from->getLevel() - (i + 1));

            //technically unsafe. Covered by assertion above
            AbstractPointer* lowerPointer = in->pointers[target.base->getName()];

            assert(lowerPointer != nullptr);

            allAssocInsts.merge(lowerPointer->getAssocInsts());
            lowerTargets.merge(lowerPointer->getTargets());
        }

        upperTargets = lowerTargets;

    }

    to->setTargets(upperTargets);

    allAssocInsts.insert((llvm::Instruction*) storeInstruction);
    allAssocInsts.insert(loadInstructions.begin(), loadInstructions.end());

    to->setAssocInsts(allAssocInsts);

    return in;

}

std::set<llvm::Instruction *> CopyOp::getAssocInstructions() {

    std::set<llvm::Instruction *> result;
    result.insert((llvm::Instruction*) storeInstruction);
    result.insert(loadInstructions.begin(), loadInstructions.end());

    return result;

}
