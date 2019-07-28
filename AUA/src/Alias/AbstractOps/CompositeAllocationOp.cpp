//
// Created by mlaupichler on 21.06.19.
//

#include "AUA/Alias/AbstractOps/CompositeAllocationOp.h"


Configuration *CompositeAllocationOp::apply(Configuration *in) {

    AbstractComposite *composite = new AbstractComposite(name, compType, dl);

    in->composites.insert_or_assign(name, composite);

    return in;
}

CompositeAllocationOp::CompositeAllocationOp(llvm::CompositeType *compType, llvm::DataLayout *dl,
                                             std::string name, llvm::AllocaInst *allocaInst)
        : compType(compType), dl(dl), name(name), allocaInst(allocaInst) {}
