nm_add_library(AUA STATIC

        src/Alias/AbstractPointers/Configuration.cpp
        src/Alias/AbstractPointers/Alias.cpp
        src/Alias/AbstractPointers/AbstractPointer.cpp
        src/Alias/AbstractPointers/AbstractTarget.cpp
        src/Alias/AbstractPointers/VarRef.cpp
        src/Alias/AbstractPointers/AbstractReference.cpp
        src/Alias/AbstractPointers/CompositeRef.cpp
        src/Alias/AbstractPointers/Finders/PointerFinder.cpp
        src/Alias/AbstractPointers/Finders/NonMemberPointerFinder.cpp
        src/Alias/AbstractPointers/Finders/MemberPointerFinder.cpp
        src/Alias/AbstractPointers/Finders/TargetFinder.cpp
        src/Alias/AbstractPointers/Finders/NonMemberTargetFinder.cpp
        src/Alias/AbstractPointers/Finders/MemberTargetFinder.cpp

        src/Alias/AbstractOps/PointerOperation.cpp
        src/Alias/AbstractOps/AssignmentOp.cpp
        src/Alias/AbstractOps/PointerAllocationOp.cpp
        src/Alias/AbstractOps/VarAllocationOp.cpp
        src/Alias/AbstractOps/CompositeAllocationOp.cpp
        src/Alias/AbstractOps/CopyOp.cpp
        src/Alias/AbstractOps/DummyFinalOp.cpp
        src/Alias/AbstractOps/DummyInitialOp.cpp
        src/Alias/AbstractOps/SinglePredAndSuccOp.cpp
        src/Alias/AbstractOps/JoinOp.cpp
        src/Alias/AbstractOps/SplitOp.cpp

)

target_include_directories(AUA PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE src)