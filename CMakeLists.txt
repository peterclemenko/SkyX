#-------------------------------------------------------------------
# This file is part of the CMake build system for SkyX
#
# The contents of this file are placed in the public domain. Feel
# free to make use of it in any way you like.
#-------------------------------------------------------------------

######################################################################
# SKYX BUILD SYSTEM
# Welcome to the CMake build system for SKYX.
# Inspired from the Ogre build system (modules FindPkgMacros.cmake,
# This is the main file where we prepare the general build environment
# and provide build configuration options.
######################################################################

cmake_minimum_required(VERSION 2.6)
cmake_policy(SET CMP0003 NEW)

project(SKYX)
# Include necessary submodules
set(CMAKE_MODULE_PATH
  "${SKYX_SOURCE_DIR}/CMake"
  "${SKYX_SOURCE_DIR}/CMake/Utils"
)

include(SkyXFindPkgMacros)
getenv_path(OGRE_HOME)

# Set where to find ogre Cmake files on unix
if (UNIX)
  set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "/usr/local/lib/OGRE/cmake/" "/usr/lib/OGRE/cmake/")
else (UNIX)
  set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ENV_OGRE_HOME}/CMake)
endif ()

include(CMakeDependentOption)
include(CheckCXXCompilerFlag)
include(SkyXMacroLogFeature)
include(SkyXConfigTargets)
set(SKYX_TEMPLATES_DIR "${SKYX_SOURCE_DIR}/CMake/Templates")
set(SKYX_WORK_DIR ${SKYX_BINARY_DIR})


#####################################################################
# Set up the basic build environment
#####################################################################

if (CMAKE_BUILD_TYPE STREQUAL "")
  # CMake defaults to leaving CMAKE_BUILD_TYPE empty. This screws up
  # differentiation between debug and release builds.
  set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "Choose the type of build, options are: None (CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif ()

# determine SkyX version numbers
include(SkyXGetVersion)
skyx_get_version(${SKYX_SOURCE_DIR}/SkyX/Include/Prerequisites.h)
message(STATUS "Configuring SKYX ${SKYX_VERSION}")

if (NOT APPLE)
  # Create debug libraries with _d postfix
  set(CMAKE_DEBUG_POSTFIX "_d")
endif ()

# Set compiler specific build flags
if (CMAKE_COMPILER_IS_GNUCXX)
#  check_cxx_compiler_flag(-msse OGRE_GCC_HAS_SSE)
#  if (OGRE_GCC_HAS_SSE)
#    add_definitions(-msse)
#  endif ()
endif ()
if (MSVC)
  add_definitions(/fp:fast)
endif ()

if (CMAKE_COMPILER_IS_GNUCXX)
  # Test for GCC visibility
  include(CheckCXXCompilerFlag)
  check_cxx_compiler_flag(-fvisibility=hidden SKYX_GCC_VISIBILITY)
  if (OGRE_GCC_VISIBILITY)
    # determine gcc version
    execute_process(COMMAND ${CMAKE_CXX_COMPILER} -dumpversion
      OUTPUT_VARIABLE SKYX_GCC_VERSION
    )
    message(STATUS "Detected g++ ${SKYX_GCC_VERSION}")
    message(STATUS "Enabling GCC visibility flags")
    set(SKYX_GCC_VISIBILITY_FLAGS "-DSKYX_GCC_VISIBILITY -fvisibility=hidden")

    # check if we can safely add -fvisibility-inlines-hidden
    string(TOLOWER "${CMAKE_BUILD_TYPE}" SKYX_BUILD_TYPE)
    if (SKYX_BUILD_TYPE STREQUAL "debug" AND SKYX_GCC_VERSION VERSION_LESS "4.2")
      message(STATUS "Skipping -fvisibility-inlines-hidden due to possible bug in g++ < 4.2")
    else ()
      set(SKYX_GCC_VISIBILITY_FLAGS "${SKYX_GCC_VISIBILITY_FLAGS} -fvisibility-inlines-hidden")
    endif ()
  endif (OGRE_GCC_VISIBILITY)

  # Fix x64 issues on Linux
  if("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "x86_64" AND NOT APPLE)
    add_definitions(-fPIC)
  endif()
endif (CMAKE_COMPILER_IS_GNUCXX)

# Add Main include path
include_directories("${SKYX_SOURCE_DIR}/SkyX/include")

# Find dependencies
include(SkyXDependencies)

# definitions for samples
set(SKYX_LIBRARIES SkyX)
if (NOT WIN32)
    set(SKYX_LIBRARIES ${SKYX_LIBRARIES} pthread)
endif(NOT WIN32)

# Specify build paths
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${SKYX_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${SKYX_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${SKYX_BINARY_DIR}/bin")
if (WIN32 OR APPLE)
  if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  # We don't want to install in default system location, install is really for the SDK, so call it for that
    set(CMAKE_INSTALL_PREFIX
      "${SKYX_BINARY_DIR}/sdk" CACHE PATH "SKYX install prefix" FORCE)
  endif (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
endif(WIN32 OR APPLE)

######################################################################
# Provide user options to customise the build process
######################################################################

# Customise what to build
option(SKYX_BUILD_SAMPLES "Build SkyX demos" TRUE)

# Customise what to install
option(SKYX_INSTALL_SAMPLES "Install SkyX demos." TRUE)
cmake_dependent_option(SKYX_INSTALL_PDB "Install debug pdb files" FALSE "MSVC" FALSE)
cmake_dependent_option(SKYX_STATIC "Static build" FALSE "OGRE_STATIC" FALSE)

###################################################################
# configure global build settings based on selected build options
###################################################################
include(SkyXConfigureBuild)

##################################################################
# Now setup targets
##################################################################

# install resource files
#include(InstallResources)

# Setup SkyX core project
add_subdirectory(SkyX)

# Setup samples
if(SKYX_BUILD_SAMPLES)
  add_subdirectory(SkyXDemo1) # Comment this line if you're using Ogre 1.8+, since this demo uses the old Ogre Terrain Scene Manager
  add_subdirectory(SkyXDemo2)
  add_subdirectory(SkyXDemo3)
endif(SKYX_BUILD_SAMPLES)

# Install CMake modules
add_subdirectory(CMake)

# Install runtime files and medias
add_subdirectory(SkyXCommon)
