# Copyright (c) 2018 Felix Kutzner
#
# This file originated from the nicemake project
# (https://github.com/fkutzner/nicemake). See README.md for details
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#
# Except as contained in this notice, the name(s) of the above copyright holders
# shall not be used in advertising or otherwise to promote the sale, use or
# other dealings in this Software without prior written authorization.

if (NOT NM_NICEMAKE_CMAKE_INCLUDED)
    cmake_minimum_required(VERSION 3.6 FATAL_ERROR)

    include(${CMAKE_CURRENT_LIST_DIR}/NiceMakeConfig.cmake)
    include(${CMAKE_CURRENT_LIST_DIR}/Platform.cmake)
    include(${CMAKE_CURRENT_LIST_DIR}/CompilerFlags.cmake)
    include(${CMAKE_CURRENT_LIST_DIR}/Sanitizers.cmake)
    include(${CMAKE_CURRENT_LIST_DIR}/Options.cmake)
    include(${CMAKE_CURRENT_LIST_DIR}/TargetCreation.cmake)
    include(${CMAKE_CURRENT_LIST_DIR}/ThirdPartyLibs.cmake)
    include(${CMAKE_CURRENT_LIST_DIR}/GTest.cmake)

    if (NOT NM_CONF_DONT_SET_OUTPUT_DIR)
        set(NM_CONF_DONT_SET_OUTPUT_DIR FALSE)
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
        set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
    endif ()

    set(NM_NICEMAKE_CMAKE_INCLUDED TRUE)
endif ()
