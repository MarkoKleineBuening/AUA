#include <utility>

//
// Created by mlaupichler on 12.05.19.
//

#include <string>
#include <AUA/Alias/AbstractPointers/AbstractPointer.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/IR/DerivedTypes.h>
#include <bits/unordered_set.h>
#include <unordered_set>
#include <sstream>


AbstractPointer::AbstractPointer(std::string n, PointerFormat format) : AbstractReference(std::move(n)), format(format) {

    assert(format.level > 0);
}


AbstractPointer::AbstractPointer(std::string n, PointerFormat format, ReferenceFlags flags) : AbstractReference(std::move(n), flags), format(format) {

    assert(format.level > 0);
}


void AbstractPointer::onlyPointTo(AbstractTarget target) {

    assert(target.base->getPointerLevel() == this->format.level - 1);

    targets.clear();
    targets.insert(target);
}


void AbstractPointer::alsoPointTo(AbstractTarget target) {

    assert(target.base->getPointerLevel() == this->format.level - 1);

    targets.insert(target);
}


void AbstractPointer::copyTargetsFrom(AbstractPointer *other) {

    assert(this->format.level == other->format.level);

    targets = other->targets;
}

void AbstractPointer::merge(AbstractPointer *other) {

    assert(this->format == other->format);

    assocInsts.merge(other->getAssociatedInsts());

    targets.merge(other->getTargets());


}


AbstractPointer *AbstractPointer::getCopy() {

    AbstractPointer *copy = new AbstractPointer(this->name, this->format, this->flags);
    copy->copyTargetsFrom(this);
    copy->assocInsts = this->assocInsts;

    return copy;

}

std::set<AbstractTarget> AbstractPointer::derefAndGetTargets(int derefDepth) {

    std::list<const llvm::Instruction *> list;
    return derefAndGetTargets(derefDepth, &list);

}


std::set<AbstractTarget> AbstractPointer::derefAndGetTargets(int derefDepth,
                                                             std::list<const llvm::Instruction *> *associatedInsts) {

    if (derefDepth < 0) throw DerefPointerLevelException();

    std::set<AbstractTarget> upperTargets;

    if (derefDepth == 0) {

        upperTargets.insert(AbstractTarget(this, 0, 0));
        return upperTargets;
    }

    upperTargets.merge(getTargets());


    for (int i = 1; i < derefDepth; ++i) {

        std::set<AbstractTarget> lowerTargets;

        for (AbstractTarget target : upperTargets) {

            assert(target.base->getPointerLevel() > 0 &&
                   target.base->getPointerLevel() == this->getPointerLevel() - i);

            //technically unsafe. Covered by assertion above
            AbstractPointer *lowerPointer = dynamic_cast<AbstractPointer *>(target.base);

            assert(lowerPointer != nullptr);

            auto lowerAssociatedInsts = lowerPointer->getAssociatedInsts();
            associatedInsts->insert(associatedInsts->end(), lowerAssociatedInsts.begin(), lowerAssociatedInsts.end());
            lowerTargets.merge(lowerPointer->getTargets());
        }

        upperTargets = lowerTargets;

    }

    return upperTargets;

}

void AbstractPointer::setTargets(const std::set<AbstractTarget> &newTargets) {

    for (auto target : newTargets) {
        assert(target.base->getPointerLevel() == this->format.level - 1);
    }

    this->targets = newTargets;

}

std::unordered_set<const llvm::Instruction *> AbstractPointer::getAssociatedInsts() {
    return assocInsts;
}

void AbstractPointer::addAssocInst(llvm::Instruction *inst) {

    assocInsts.insert(inst);

}

void AbstractPointer::addAllAssocInsts(std::list<const llvm::Instruction *> insts) {

    for (auto inst : insts) {
        assocInsts.insert(inst);
    }

}

void AbstractPointer::setAssocInsts(const std::list<const llvm::Instruction *> &newAssocInsts) {

    assocInsts.clear();
    for (auto inst : newAssocInsts) {
        assocInsts.insert(inst);
    }

}

void AbstractPointer::setOnlyAssocInst(llvm::Instruction *assocInst) {
    assocInsts.clear();
    assocInsts.insert(assocInst);
}

const PointerFormat &AbstractPointer::getFormat() const {
    return format;
}

void AbstractPointer::addTargets(std::set<AbstractTarget> targetsToAdd) {

    this->targets.merge(targetsToAdd);

}

const std::string AbstractPointer::to_string() {

    std::ostringstream oss;
    oss << name << " (level: " << getPointerLevel() << ", flags: " << flags.to_string() << ")";

    return oss.str();

}

const int AbstractPointer::getSize() {
    return 0;
}
