#=============================================================================
# Copyright (c) 2015 Frozen Team.
#
# Permission is hereby granted, free of charge, to any person obtaining
# a copy of this software and associated documentation files (the "Software"),
# to deal in the Software without restriction, including without limitation 
# the rights to use, copy, modify, merge, publish, distribute, sublicense, 
# and/or sell copies of the Software, and to permit persons to whom the 
# Software is furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included 
# in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE 
# SOFTWARE.
#==============================================================================
# Locate FBXSDK
# This module defines:
# FBXSDK_INCLUDE_DIR, where to find the headers
#
# FBXSDK_LIBRARY, FBXSDK_LIBRARY_DEBUG
# FBXSDK_FOUND
#
# $FBXSDK_DIR is an environment variable that
# would correspond to the installation path.
#
# TODO list: 
# 1. Find in OSX, Linux

if(APPLE)
    set(FBXSDK_LIBDIR "gcc4/ub")
elseif(CMAKE_COMPILER_IS_GNUCXX)
    set(FBXSDK_LIBDIR "gcc4")
elseif(MSVC12)
    set(FBXSDK_LIBDIR "vs2013")
elseif(MSVC14 OR MSVC_VERSION>1900)
    set(FBXSDK_LIBDIR "vs2015")
endif()

if(NOT DEFINED ENGINE_DEFAULT_FBX_VERSION)
    set(ENGINE_DEFAULT_FBX_VERSION "2016.1.2")
endif()

if(APPLE)
    # do nothing
elseif(CMAKE_CL_64)
    set(FBXSDK_LIBDIR ${FBXSDK_LIBDIR}/x64)
elseif(CMAKE_COMPILER_IS_GNUCXX AND CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(FBXSDK_LIBDIR ${FBXSDK_LIBDIR}/x64)
else()
    set(FBXSDK_LIBDIR ${FBXSDK_LIBDIR}/x86)
endif()

if(APPLE)
    set(FBXSDK_LIBNAME "libfbxsdk")
elseif(CMAKE_COMPILER_IS_GNUCXX)
    set(FBXSDK_LIBNAME "fbxsdk")
else()
    set(FBXSDK_LIBNAME "libfbxsdk-md")
endif()

set(FBXSDK_SEARCH_PATHS
    $ENV{FBXSDK_DIR}
    "$ENV{ProgramW6432}/Autodesk/FBX/FBX SDK/2016.1.2"
    "$ENV{PROGRAMFILES}/Autodesk/FBX/FBX SDK/2016.1.2"
)

# Find path to include directory.
find_path(FBXSDK_INCLUDE_DIR "fbxsdk.h"
    PATHS ${FBXSDK_SEARCH_PATHS}
    PATH_SUFFIXES "include"
)

# Find release library
find_library(FBXSDK_LIBRARY ${FBXSDK_LIBNAME}
    PATHS ${FBXSDK_SEARCH_PATHS}
    PATH_SUFFIXES "lib/${FBXSDK_LIBDIR}/release"
)

# Find debug library
find_library(FBXSDK_LIBRARY_DEBUG ${FBXSDK_LIBNAME}
    PATHS ${FBXSDK_SEARCH_PATHS}
    PATH_SUFFIXES "lib/${FBXSDK_LIBDIR}/debug")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(FBXSDK REQUIRED_VARS FBXSDK_LIBRARY FBXSDK_LIBRARY_DEBUG FBXSDK_INCLUDE_DIR)

if(FBXSDK_FOUND)
    set(CMAKE_EXE_LINKER_FLAGS /NODEFAULTLIB:\"LIBCMT\")
    list(APPEND FBXSDK_LIBRARY Wininet.lib)
    list(APPEND FBXSDK_LIBRARY_DEBUG Wininet.lib)
endif()