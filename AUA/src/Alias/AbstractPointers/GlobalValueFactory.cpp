//
// Created by mlaupichler on 02.08.19.
//

#include "AUA/Alias/AbstractPointers/GlobalValueFactory.h"

GlobalValueFactory::GlobalValueFactory(llvm::Module *module)
    : module(module), dl(new llvm::DataLayout(module)) {}

AbstractPointer *GlobalValueFactory::buildGlobalAbstractPointer(const std::string& name) {

    llvm::GlobalVariable* g = module->getGlobalVariable(name);

    if (g == nullptr) throw GlobalVariableNotExistingException(name);

    llvm::Type* type = g->getType();

    if (!llvm::isa<llvm::PointerType>(type)) throw GlobalVariableTypeMatchException(name, "pointer");

    auto ptrType = llvm::cast<llvm::PointerType>(type);

    return new AbstractPointer(g->getName(), PointerFormat(ptrType), globalFlags);

}

AbstractComposite *GlobalValueFactory::buildGlobalAbstractComposite(const std::string& name) {

    llvm::GlobalVariable* g = module->getGlobalVariable(name);

    if (g == nullptr) throw GlobalVariableNotExistingException(name);

    llvm::Type* type = g->getType();

    if (!llvm::isa<llvm::CompositeType>(type)) throw GlobalVariableTypeMatchException(name, "composite");

    auto compType = llvm::cast<llvm::CompositeType>(type);

    return new AbstractComposite(name, CompositeFormat(compType, const_cast<llvm::DataLayout *&>(dl)), globalFlags);

}

AbstractVar *GlobalValueFactory::buildGlobalAbstractVar(const std::string& name) {

    llvm::GlobalVariable* g = module->getGlobalVariable(name);

    if (g == nullptr) throw GlobalVariableNotExistingException(name);

    llvm::Type* type = g->getType();

    if (llvm::isa<llvm::PointerType>(type) || llvm::isa<llvm::CompositeType>(type)) throw GlobalVariableTypeMatchException(name, "plain var (no pointer or composite)");

    return new AbstractVar(name, dl->getTypeAllocSize(type), globalFlags);

}

bool GlobalValueFactory::globalPointerExists(const std::string &name) {

    llvm::GlobalVariable* g = module->getGlobalVariable(name);

    if (g == nullptr) return false;

    llvm::Type* type = g->getType();

    return llvm::isa<llvm::PointerType>(type);

}

bool GlobalValueFactory::globalCompositeExists(const std::string &name) {

    llvm::GlobalVariable* g = module->getGlobalVariable(name);

    if (g == nullptr) return false;

    llvm::Type* type = g->getType();

    return llvm::isa<llvm::CompositeType>(type);

}

bool GlobalValueFactory::globalVarExists(const std::string &name) {

    llvm::GlobalVariable* g = module->getGlobalVariable(name);

    if (g == nullptr) return false;

    llvm::Type* type = g->getType();

    return !(llvm::isa<llvm::CompositeType>(type) || llvm::isa<llvm::PointerType>(type));

}


