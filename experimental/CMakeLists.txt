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

if(SST_ENABLE_HDF5)
  find_package(HDF5 REQUIRED)
endif(SST_ENABLE_HDF5)

if(NOT SST_DISABLE_MPI)
  find_package(MPI REQUIRED)
endif(NOT SST_DISABLE_MPI)

find_package(ZLIB)

find_package(Python 3 REQUIRED COMPONENTS Development Interpreter)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
# Defines everything we need for sst_config.h
include(header_config)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../src ${CMAKE_CURRENT_BINARY_DIR}/src)
