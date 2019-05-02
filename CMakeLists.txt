cmake_minimum_required (VERSION 3.10)
project(AUA)

include(cmake/NiceMake/NiceMake.cmake)

include(cmake/SAMThirdPartyLibs.cmake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#nm_add_doxygen()

add_subdirectory(lib)

add_subdirectory(tools)
