//
// Created by mlaupichler on 12.07.19.
//

#include <AUA/Alias/AbstractPointers/SetValues/SetValueFormatException.h>
#include "AUA/Alias/AbstractPointers/SetValues/PointerSetValue.h"

bool PointerSetValue::formatCheck(AbstractPointer *ptr) {
    return (ptr->getFormat() == this->format);
}

bool PointerSetValue::include(AbstractPointer *el) {

    if (!formatCheck(el)) {

        llvm::outs() << el->getName() << ": " << el->getFormat().level << " =/= " << format.level << "\n";
        throw SetValueFormatException();
    }

    if (contents.find(el->getName()) != contents.end()) return false;

    contents[el->getName()] = el;
    return true;

}

bool PointerSetValue::exclude(AbstractPointer *el) {

    if (!formatCheck(el)) throw SetValueFormatException();

    if (contents.find(el->getName()) == contents.end()) return false;

    contents.erase(el->getName());
    return true;

}

int PointerSetValue::size() {
    return contents.size();
}

std::set<AbstractTarget> PointerSetValue::getMergedTargets() {
    return getMergedTargets(1);
}

std::set<AbstractTarget> PointerSetValue::getMergedTargets(int derefDepth) {

    assert(derefDepth <= format.level);

    std::set<AbstractTarget> result;

    for (auto ptrPair : contents) {

        result.merge(ptrPair.second->derefAndGetTargets(derefDepth));

    }

    return result;

}

std::set<AbstractPointer *> PointerSetValue::asSet() {

    std::set<AbstractPointer *> result;

    for (auto elPair : contents) {
        result.insert(elPair.second);
    }

    return result;

}

PointerSetValue::PointerSetValue(PointerFormat format) : format(format) {
    assert(format.level > 0);
}

PointerSetValue::PointerSetValue(PointerFormat format, std::set<AbstractPointer *> initial) : format(format) {

    assert(format.level > 0);

    for (auto ptr : initial) {

        include(ptr);

    }

}

const PointerFormat &PointerSetValue::getFormat() const {
    return format;
}

AbstractPointer *PointerSetValue::mergeToNewPointer(std::string name) {

    AbstractPointer *result = new AbstractPointer(name, format);
    result->setTargets(getMergedTargets());
    return result;

}
