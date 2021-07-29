# Set variables for sst_config.h
set(SST_BUILD_WITH_CMAKE ON)
set(SST_CXXFLAGS ${CMAKE_CXX_FLAGS})
set(SST_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
set(PACKAGE_VERSION ${CMAKE_PROJECT_VERSION})

# TODO CHECK FOR THIS
set(__STDC_FORMAT_MACROS ON)

if(Python_VERSION AND Python_VERSION_MAJOR GREATER_EQUAL 3)
  set(SST_CONFIG_HAVE_PYTHON3 ON)
  set(HAVE_PYTHON_H ON)
endif(Python_VERSION AND Python_VERSION_MAJOR GREATER_EQUAL 3)

if(APPLE)
  set(SST_COMPILE_MACOSX ON)
endif(APPLE)

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
check_library_exists(m sin "" HAVE_LIBM)
if(NOT HAVE_LIBM)
  message(FATAL_ERROR "Failed to detect libm")
endif(NOT HAVE_LIBM)

check_library_exists(ltdl lt_dlinit "" HAVE_LTDL_EXTERNAL)
if(NOT HAVE_LTDL_EXTERNAL)
  include(ExternalProject)
  set(LIBLTDL_INSTALL_DIRS "${CMAKE_BINARY_DIR}/contrib/install/" 
    CACHE PATH "Path to the libldl install")
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
    INSTALL_DIR ${LIBLTDL_INSTALL_DIRS}
    CONFIGURE_COMMAND aclocal && autoconf && automake && ./configure --prefix=${LIBLTDL_INSTALL_DIRS} --quiet --enable-ltdl-install
    BUILD_COMMAND make --quiet
    INSTALL_COMMAND make --quiet install
  )
  # TODO this is probably not the right way to set these
  set(LIBLTDL_INCLUCE_DIRS "${LIBLTDL_INSTALL_DIRS}/include" 
    CACHE PATH "Location of the installed libltdl headers")
  set(LIBLTDL_LIBRARIES "${LIBLTDL_INSTALL_DIRS}/lib/libltdl.a" 
    CACHE FILEPATH "Path to the install libltdl library")
else(NOT HAVE_LTDL_EXTERNAL)
  add_custom_target(libltdl)
  set(LIBLTDL_INCLUCE_DIRS "")
  set(LIBLTDL_LIBRARIES "ltdl")
endif(NOT HAVE_LTDL_EXTERNAL)
message(STATUS "libltdl includes ${LIBLTDL_INCLUCE_DIRS}.")
message(STATUS "libltdl library ${LIBLTDL_LIBRARIES}.")
message(STATUS "libltdl install ${LIBLTDL_INSTALL_DIRS}.")
