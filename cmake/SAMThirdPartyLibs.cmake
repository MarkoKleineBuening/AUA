include_guard()

# Add LLVM
find_package(LLVM REQUIRED CONFIG)
list(REMOVE_ITEM LLVM_AVAILABLE_LIBS "LTO")
nm_add_thirdparty_libs(NAME sam_llvm
                          LIBS ${LLVM_AVAILABLE_LIBS}
                          INTERFACE_INCLUDE_DIRS ${LLVM_INCLUDE_DIRS}
                          INTERFACE_COMPILE_OPTIONS -D_DEBUG
                                                    -D__STDC_CONSTANT_MACROS
                                                    -D__STDC_FORMAT_MACROS
                                                    -D__STDC_LIMIT_MACROS)