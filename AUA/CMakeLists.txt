nm_add_library(AUA STATIC

        src/Alias/AbstractPointers/Configuration.cpp
        src/Alias/AbstractPointers/GlobalConfiguration.cpp
        src/Alias/AbstractPointers/ConfigurationPrinter.cpp
        src/Alias/AbstractPointers/Alias.cpp

        src/Alias/AbstractPointers/AbstractTarget.cpp

        src/Alias/AbstractPointers/AbstractReference.cpp
        src/Alias/AbstractPointers/AbstractVar.cpp
        src/Alias/AbstractPointers/AbstractPointer.cpp
        src/Alias/AbstractPointers/AbstractComposite.cpp

        src/Alias/AbstractPointers/AbstractFunction.cpp
        src/Alias/AbstractPointers/AbstractFunctionFactory.cpp

        src/Alias/AbstractPointers/Finders/FinderFactory.cpp

        src/Alias/AbstractPointers/Finders/PointerFinder.cpp
        src/Alias/AbstractPointers/Finders/BasePointerFinder.cpp
        src/Alias/AbstractPointers/Finders/GlobalPointerFinder.cpp
        src/Alias/AbstractPointers/Finders/FromPointerPointerFinder.cpp
        src/Alias/AbstractPointers/Finders/MemberPointerFinder.cpp
        src/Alias/AbstractPointers/Finders/ReturnedPointerFinder.cpp
        src/Alias/AbstractPointers/Finders/AnonymousPointerFinder.cpp

        src/Alias/AbstractPointers/Finders/CompositeFinder.cpp
        src/Alias/AbstractPointers/Finders/BaseCompositeFinder.cpp
        src/Alias/AbstractPointers/Finders/GlobalCompositeFinder.cpp
        src/Alias/AbstractPointers/Finders/FromPointerCompositeFinder.cpp
        src/Alias/AbstractPointers/Finders/MemberCompositeFinder.cpp

        src/Alias/AbstractPointers/Finders/TargetFinder.cpp
        src/Alias/AbstractPointers/Finders/BaseTargetFinder.cpp
        src/Alias/AbstractPointers/Finders/GlobalTargetFinder.cpp
        src/Alias/AbstractPointers/Finders/FromPointerTargetFinder.cpp
        src/Alias/AbstractPointers/Finders/MemberTargetFinder.cpp


        src/Alias/AbstractPointers/Finders/FunctionFinder.cpp
        src/Alias/AbstractPointers/Finders/GlobalFunctionFinder.cpp


        src/Alias/AbstractPointers/SetValues/SetValue.cpp
        src/Alias/AbstractPointers/SetValues/PointerSetValue.cpp
        src/Alias/AbstractPointers/SetValues/CompositeSetValue.cpp


        src/Alias/AbstractPointers/PointerFormat.cpp
        src/Alias/AbstractPointers/CompositeFormat.cpp


        src/Alias/AbstractOps/PointerOperation.cpp
        src/Alias/AbstractOps/AssignmentOp.cpp
        src/Alias/AbstractOps/PointerAllocationOp.cpp
        src/Alias/AbstractOps/VarAllocationOp.cpp
        src/Alias/AbstractOps/CompositeAllocationOp.cpp
        src/Alias/AbstractOps/StoreOp.cpp
        src/Alias/AbstractOps/CallWithIrrelevantReturnOp.cpp
        src/Alias/AbstractOps/ReturnOp.cpp
        src/Alias/AbstractOps/ReturnNilOp.cpp
        src/Alias/AbstractOps/ReturnPointerOp.cpp
        src/Alias/AbstractOps/DummyInitialOp.cpp
        src/Alias/AbstractOps/SinglePredAndSuccOp.cpp
        src/Alias/AbstractOps/JoinOp.cpp
        src/Alias/AbstractOps/SplitOp.cpp

        )

target_include_directories(AUA PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE src)