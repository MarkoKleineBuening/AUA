#include <utility>

//
// Created by mlaupichler on 17.07.19.
//

#include "AUA/Alias/AbstractPointers/Finders/FromPointerPointerFinder.h"

PointerSetValue *FromPointerPointerFinder::findPointers(Configuration *configuration) const {

    PointerSetValue *topLevelPointers = topLevelPointerFinder->findPointers(configuration);
    auto pointerTargets = topLevelPointers->getMergedTargets(derefDepth);

    auto result = new PointerSetValue(PointerFormat(expectedFormat));

    for (AbstractTarget target : pointerTargets) {

        auto resultPointer = dynamic_cast<AbstractPointer *>(target.base);

        assert(resultPointer != nullptr);

        result->include(resultPointer);
    }

    return result;

}

FromPointerPointerFinder::FromPointerPointerFinder(const PointerFinder *topLevelPointerFinder, const int derefDepth,
                                                   const PointerFormat &expectedFormat,
                                                   const std::list<const llvm::LoadInst *>& loadInsts)
        : PointerFinder(expectedFormat), derefDepth(derefDepth), topLevelPointerFinder(topLevelPointerFinder),
          loadInsts(loadInsts) {}

std::list<const llvm::Instruction *> FromPointerPointerFinder::getAssociatedInsts() const {

    auto result = topLevelPointerFinder->getAssociatedInsts();
    result.insert(result.end(), loadInsts.begin(), loadInsts.end());

    return result;

}
