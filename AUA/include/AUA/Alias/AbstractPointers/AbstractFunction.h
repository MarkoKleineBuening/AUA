//
// Created by mlaupichler on 10.07.19.
//

#ifndef AUA_ABSTRACTFUNCTION_H
#define AUA_ABSTRACTFUNCTION_H


#include <AUA/Alias/AbstractPointers/SetValues/PointerSetValue.h>
#include <AUA/Alias/AbstractPointers/Configuration.h>
#include <AUA/Alias/AbstractPointers/AbstractPointer.h>
#include "AUA/Alias/AbstractPointers/SetValues/CompositeSetValue.h"
#include <AUA/Alias/AbstractOps/DummyInitialOp.h>
#include <AUA/Alias/AbstractOps/ReturnOp.h>


class AbstractFunction {

private:

    std::string name;

    const std::vector<std::string> paramNames;

    DummyInitialOp *initialOp;

    ReturnOp *finalOp;

    Configuration* lastConfiguration = nullptr;

    const std::map<int, PointerFormat>* ptrParamFormats;
    const std::map<int, CompositeFormat>* compParamFormats;

    const std::set<AbstractVar*>* varParams;

    std::map<int, PointerSetValue*> getEmptyInputPointers();
    std::map<int, CompositeSetValue*> getEmptyInputComposites();


public:

    AbstractFunction(DummyInitialOp *initialOp, ReturnOp *finalOp, std::string name,
                     std::map<int, PointerFormat> *ptrParamFormats,
                     std::map<int, CompositeFormat> *compParamFormats,
                     const std::set<AbstractVar*>* varParams,
                     std::vector<std::string> paramNames);

    const std::string &getName() const;

    Configuration *getLastConfiguration() const;

    /**
     * Executes alias analysis on this AbstractFunction with the given pointer and composite parameters with their parameter indices.
     * Parameter pointers remain unchanged, values are copied for use in AbstractFunction.
     * @param ptrParams the pointer parameters given with their parameter index in this function.
     * @param compParams the composite parameters given with their parameter index in this function.
     * @return the pointer that is returned by the AbstractFunction for further use in the callers alias analysis or
     * nullptr if the function has no pointer type return value.
     */
    PointerSetValue *execute(std::map<int, PointerSetValue *> ptrParams,
                             std::map<int, CompositeSetValue *> compParams);

    /**
     * Executes alias analysis on this function as the entry function, i.e. all parameters are default set to empty values.
     */
    void executeAsEntry();
};


#endif //AUA_ABSTRACTFUNCTION_H
