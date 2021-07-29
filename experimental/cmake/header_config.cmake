# Set variables for sst_config.h
set(SST_BUILD_WITH_CMAKE ON)
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

include(CheckLibraryExists)
check_library_exists(ltdl lt_dlinit "" HAVE_LTDL)

set(FOUND_EXTERNAL_LIBLTDL FALSE)
if(NOT HAVE_LTDL)
  include(ExternalProject)
  if(NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/libltdl)
    find_program(LIBTOOLIZE NAMES libtoolize glibtoolize)
    message(STATUS "Using ${LIBTOOLIZE} as libtoolize.")
    execute_process(
      COMMAND ${LIBTOOLIZE} --quiet --ltdl=${CMAKE_CURRENT_BINARY_DIR}/libltdl
      RESULT_VARIABLE LIBTOOLIZE_RESULT
      )

    if(LIBTOOLIZE_RESULT AND NOT LIBTOOLIZE_RESULT EQUAL 0)
      message(FATAL_ERROR 
        "${LIBTOOLIZE} failed to generate libltdl with result ${LIBTOOLIZE_RESULT}.")
    endif(LIBTOOLIZE_RESULT AND NOT LIBTOOLIZE_RESULT EQUAL 0)
  endif(NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/libltdl)

  ExternalProject_Add(libltdl
    SOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/libltdl
    BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/libltdl
    INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/contrib/libltdl
    CONFIGURE_COMMAND aclocal && autoconf && automake && ./configure --prefix=${CMAKE_CURRENT_BINARY_DIR}/contrib/libltdl --quiet --enable-ltdl-install
    BUILD_COMMAND make --quiet
    INSTALL_COMMAND make --quiet install
  )
else(NOT HAVE_LTDL)
  set(FOUND_EXTERNAL_LIBLTDL TRUE)
  add_custom_target(libltdl)
endif(NOT HAVE_LTDL)

