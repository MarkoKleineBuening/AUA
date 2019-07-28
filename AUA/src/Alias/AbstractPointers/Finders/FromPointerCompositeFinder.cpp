#include <utility>

//
// Created by mlaupichler on 18.07.19.
//

#include "AUA/Alias/AbstractPointers/Finders/FromPointerCompositeFinder.h"

FromPointerCompositeFinder::FromPointerCompositeFinder(const PointerFinder *pointerFinder,
                                                       const CompositeFormat& expectedFormat,
                                                       const llvm::LoadInst *loadInst)
        : CompositeFinder(expectedFormat), pointerFinder(pointerFinder), loadInst(loadInst) {}

CompositeSetValue *FromPointerCompositeFinder::findComposites(Configuration *configuration) const {

    PointerSetValue *pointers = pointerFinder->findPointers(configuration);
    CompositeSetValue *result = new CompositeSetValue(expectedFormat);

    for (auto compTarget : pointers->getMergedTargets()) {

        AbstractComposite *comp = dynamic_cast<AbstractComposite *>(compTarget.base);
        result->include(comp);

    }

    return result;


}

std::list<const llvm::Instruction *> FromPointerCompositeFinder::getAssociatedInsts() const {

    auto result = pointerFinder->getAssociatedInsts();
    result.push_back(loadInst);

    return result;

}
