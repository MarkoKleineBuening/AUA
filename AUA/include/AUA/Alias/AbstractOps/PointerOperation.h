//
// Created by mlaupichler on 16.05.19.
//

#ifndef AUA_POINTEROPERATION_H
#define AUA_POINTEROPERATION_H

#include <map>
#include <AUA/Alias/AbstractPointers/AbstractPointer.h>
#include <AUA/Alias/Configuration.h>

/**
 * Abstract class for abstracting llvm instructions to operations that influence pointers.
 */
class PointerOperation {

protected:

    /**
     * Consume the result Configuration of this PointerOperation.
     * @param result the result of this PointerOperation.
     */
    void consume(Configuration* result);


    /**
     * Applies the operation on a given Configuration and returns the resulting Configuration.
     * @param in the Configuration to apply this Operation to.
     * @return the resulting Configuration.
     */
    virtual Configuration* apply(Configuration* in) = 0;

public:

    /**
     * Executes this operation on a given configuration and executes all following PointerOperations if any exist.
     * @param in the configuration to work on.
     */
    virtual void execute(Configuration* in) = 0;

    /**
     * Links this PointerOperation to a following operation. Makes this operation a predecessor of the other operation and the other operation a successor of this one.
     * @param successor the operation that is a successor of this operation.
     */
    virtual void linkSuccessor(PointerOperation* successor) = 0;

    /**
     * Adds a predecessor to this PointerOperation. Does NOT add this operation as successor to the given operation.
     * @param predecessor the operation that is a predecessor of this operation.
     */
    virtual void addPredecessor(PointerOperation *predecessor) = 0;

    /**
     * Returns all succeeding PointerOperations to this operation.
     * @return the succeeding operations.
     */
    virtual std::set<PointerOperation*> getSuccessors() = 0;


    virtual std::set<PointerOperation*> getPredecessors() = 0;



};


#endif //AUA_POINTEROPERATION_H
