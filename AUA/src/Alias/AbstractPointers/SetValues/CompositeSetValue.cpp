//
// Created by mlaupichler on 12.07.19.
//

#include "AUA/Alias/AbstractPointers/SetValues/CompositeSetValue.h"
#include "AUA/Alias/AbstractPointers/SetValues/SetValueFormatException.h"
#include "AUA/Alias/AbstractPointers/SetValues/PointerSetValue.h"

bool CompositeSetValue::formatCheck(AbstractComposite *composite) {

    return (this->format == composite->getFormat());

}

CompositeSetValue::CompositeSetValue(CompositeFormat format) : format(format) {}

CompositeSetValue::CompositeSetValue(CompositeFormat format, std::set<AbstractComposite *> initial) : format(format) {

    for (auto comp : initial) {

        include(comp);

    }

}

AbstractComposite *CompositeSetValue::mergeToNewComposite(std::string name, ReferenceFlags flags) {

    AbstractComposite *result = new AbstractComposite(name, format, flags);

    for (auto compPair : contents) {
        result->merge(compPair.second);
    }

    return result;

}

const CompositeFormat &CompositeSetValue::getFormat() const {
    return format;
}

bool CompositeSetValue::include(AbstractComposite *el) {

    if (!formatCheck(el)) throw SetValueFormatException();

    if (contents.find(el->getName()) != contents.end()) return false;

    contents[el->getName()] = el;
    return true;

}

bool CompositeSetValue::exclude(AbstractComposite *el) {

    if (!formatCheck(el)) throw SetValueFormatException();

    if (contents.find(el->getName()) == contents.end()) return false;

    contents.erase(el->getName());
    return true;

}

int CompositeSetValue::size() {
    return contents.size();
}

std::set<AbstractComposite *> CompositeSetValue::asSet() {

    std::set<AbstractComposite *> result;

    for (auto elPair : contents) {
        result.insert(elPair.second);
    }

    return result;

}



