cmake_minimum_required(VERSION 3.17..3.21)

project(SST VERSION 12.0.0
  DESCRIPTION "SST" LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 11 CACHE STRING "C++ standard to conform to")
set(CMAKE_CXX_STANDARD_REQUIRED YES)

if( CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)
  message(FATAL_ERROR "In-source builds are not allowed.
Please create a directory and run cmake from there, passing the path
to this source directory as the last argument.
This process created the file `CMakeCache.txt' and the directory `CMakeFiles'.
Please delete them.")
endif()

#find_package(HDF5)
#find_package(MPI)
#find_package(ZLIB)
find_package(Python REQUIRED COMPONENTS Development Interpreter)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
# Defines everything we need for sst_config.h
include(header_config)

# Set variables for sst_config.h
set(SST_CXXFLAGS ${CMAKE_CXX_FLAGS})
set(SST_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
set(PACKAGE_VERSION ${CMAKE_PROJECT_VERSION})
execute_process(
         COMMAND git rev-parse HEAD
         RESULT_VARIABLE HASH_RESULT
         OUTPUT_VARIABLE SSTCORE_GIT_HEADSHA
         OUTPUT_STRIP_TRAILING_WHITESPACE
         )
execute_process(
         COMMAND git branch --show-current
         RESULT_VARIABLE BRANCH_RESULT
         OUTPUT_VARIABLE SSTCORE_GIT_BRANCH
         OUTPUT_STRIP_TRAILING_WHITESPACE
         )

include(CheckIncludeFile)
check_include_file(sys/stat.h HAVE_SYS_STAT_H)
check_include_file(dlfcn.h HAVE_DLFCN_H)
check_include_file(sys/types HAVE_SYS_TYPES_H)
check_include_file(unistd.h HAVE_UNISTD_H)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../src ${CMAKE_CURRENT_BINARY_DIR}/src)
