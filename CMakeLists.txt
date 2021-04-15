cmake_minimum_required(VERSION 2.6)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set (CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR} CACHE PATH "installation root" FORCE)


set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/share)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

project(Hades-Workshop)

enable_testing()

set(HADES_MAJOR_VERSION 0)
set(HADES_MINOR_VERSION 41)
set(HADES_PATCH_VERSION "e")
set(HADES_VERSION "${HADES_MAJOR_VERSION}.${HADES_MINOR_VERSION}${HADES_PATCH_VERSION}")

IF( DEFINED ENV{CIBranch})
SET(HADES_VERSION ${HADES_VERSION}.$ENV{CIBranch})
ENDIF()

IF( DEFINED ENV{CICurrentBuild})
SET(HADES_VERSION ${HADES_VERSION}.Build$ENV{CICurrentBuild})
ENDIF()

IF( DEFINED ENV{CICommitHash})
SET(HADES_VERSION ${HADES_VERSION}.$ENV{CICommitHash})
ENDIF()

message(STATUS "Haded Workshop Version ${HADES_VERSION}")

set(CMAKE_BUILD_TYPE RELEASE)
set(CMAKE_VERBOSE_MAKEFILE FALSE)
set(CMAKE_SKIP_RPATH TRUE)

IF(NOT INSTALL_ROOT AND NOT $ENV{INSTALL_ROOT} STREQUAL "")
   SET(INSTALL_ROOT $ENV{INSTALL_ROOT})
ENDIF()

option(USE_NSIS "Use NSIS generator to produce installer" OFF)

# Custom target for packaging.
if(USE_NSIS)
  set(CPACK_GENERATOR "NSIS")
else(USE_NSIS)
  set(CPACK_GENERATOR "ZIP")
endif(USE_NSIS)



set(CPACK_PACKAGE_VERSION "${HADES_VERSION}")
set(CPACK_PACKAGE_VERSION_PATCH "${HADES_PATCH_VERSION}")
set(CPACK_PACKAGE_INSTALL_REGISTRY_KEY "HADES")

set(CPACK_SOURCE_GENERATOR "TGZ")


# Compiler flags
if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    # Disable some of the bullshit warnings MSVC wants to barf
    add_definitions( "-W3 -MP -D_CRT_SECURE_NO_WARNINGS -wd4005 -wd4996 -wd4305 -wd4244 -nologo" )
else()
    set(CMAKE_CXX_FLAGS_RELEASE "-msse4 -mavx -mlzcnt -O3 -fvisibility=hidden -fPIC -std=c++11")
    add_definitions(-D__LZCNT__ -D_LINUX)
endif()

# check if we have a local cmake include file and include it if we do
# this is useful for setting our arnold location as an alternative to
# environment variables
if(EXISTS ${CMAKE_SOURCE_DIR}/local.cmake)
    message(STATUS "Using local.cmake")
    include(${CMAKE_SOURCE_DIR}/local.cmake)
endif()

# Edit available Configrations to make them available in IDE that support multiple-configuration (for example Visual Studio)
if(CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_CONFIGURATION_TYPES Debug Release RelWithDebInfo)
  set(CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}" CACHE STRING
      "Reset the configurations to what we need" FORCE)
endif()


add_subdirectory("Source")
add_subdirectory("thirdParty/wxWidgets")

