# Set variables for sst_config.h attempt to just go in order
include(CheckSymbolExists)
include(CheckIncludeFile)
include(CheckLibraryExists)
check_symbol_exists(argz_add "argz.h" HAVE_ARGZ_ADD)
check_symbol_exists(argz_append "argz.h" HAVE_ARGZ_APPEND)
check_symbol_exists(argz_count "argz.h" HAVE_ARGZ_COUNT)
check_symbol_exists(argz_create_sep "argz.h" HAVE_ARGZ_CREATE_SEP)
check_include_file(argz.h HAVE_ARGZ_H)
check_symbol_exists(argz_insert "argz.h" HAVE_ARGZ_INSERT)
check_symbol_exists(argz_next "argz.h" HAVE_ARGZ_NEXT)
check_symbol_exists(argz_stringify "argz.h" HAVE_ARGZ_STRINGIFY)
check_symbol_exists(closedir "sys/types.h" HAVE_CLOSEDIR)
check_include_file(c_asm.h HAVE_C_ASM_H)
check_symbol_exists(cygwin_conv_path "sys/cygwin.h" HAVE_DECL_CYGWIN_CONV_PATH)
check_include_file(dirent.h HAVE_DIRENT_H)
check_library_exists(dld dld_init "" HAVE_DLD)
check_include_file(dld.h HAVE_DLD_H)
check_symbol_exists(dlerror "dlfcn.h" HAVE_DLERROR)
check_include_file(dlfcn.h HAVE_DLFCN_H)
check_include_file(dl.h HAVE_DL_H)
check_symbol_exists(_dyld_func_lookup "dyld.h" HAVE_DYLD)
check_symbol_exists(error_t "errno.h" HAVE_ERROR_T)
if(HDF5_FOUND)
  set(HAVE_HDF5 ON)
endif(HDF5_FOUND)
check_include_file(intrinsics.h HAVE_INTRINSICS_H)
check_include_file(inttypes.h HAVE_INTTYPES_H)
if(CMAKE_DL_LIBS)
  set(HAVE_LIBDL ON)
endif(CMAKE_DL_LIBS)
if(ZLIB_FOUND)
  set(HAVE_LIBZ ON)
endif(ZLIB_FOUND)
check_include_file(mach/mach_time.h HAVE_MACH_MACH_TIME_H)
check_include_file(mach-o/dyld.h HAVE_MACH_O_DYLD_H)
check_symbol_exists(opendir "dirent.h" HAVE_OPENDIR)
if(Python_FOUND)
  set(HAVE_PYTHON_H 1)
endif(Python_FOUND)
check_symbol_exists(readdir "dirent.h" HAVE_READDIR)
if(CMAKE_CXX_STANDARD EQUAL 14)
  set(HAVE_STDCXX_1Y ON)
endif(CMAKE_CXX_STANDARD EQUAL 14)
check_include_file(stdint.h HAVE_STDINT_H)
check_include_file(stdio.h HAVE_STDIO_H)
check_include_file(stdlib.h HAVE_STDLIB_H)
check_include_file(strings.h HAVE_STRINGS_H)
check_include_file(string.h HAVE_STRING_H)
check_symbol_exists(strlcat "string.h" HAVE_STRLCAT)
check_symbol_exists(strlcpy "string.h" HAVE_STRLCPY)
check_include_file(sys/dl.h HAVE_SYS_DL_H)
check_include_file(sys/stat.h HAVE_SYS_STAT_H)
check_include_file(sys/stat.h HAVE_SYS_STAT_H)
check_include_file(sys/time.h HAVE_SYS_TIME_H)
check_include_file(sys/types.h HAVE_SYS_TYPES_H)
check_include_file(unistd.h HAVE_UNISTD_H)
if(HAVE_ARGZ_H)
  set(HAVE_WORKING_ARGZ ON)
endif(HAVE_ARGZ_H)
set(PACKAGE_BUGREPORT "https://github.com/sstsimulator/sst-core/issues")
set(PACKAGE_NAME "SSTCore")
set(PACKAGE_NAME "https://github.com/sstsimulator/sst-core")
set(SST_CC ${CMAKE_C_COMPILER})
set(SST_CFLAGS ${CMAKE_C_FLAGS})
if(APPLE)
  set(SST_COMPILE_MACOSX 1)
endif(APPLE)
if(MPI_FOUND)
  set(SST_CONFIG_HAVE_MPI ON)
endif(MPI_FOUND)
if(Python_FOUND)
  set(SST_CONFIG_HAVE_PYTHON ON)
  set(SST_CONFIG_HAVE_PYTHON3 ON)
endif(Python_FOUND)
set(SST_CPP ${CMAKE_CXX_COMPILER})
set(SST_CPPFLAGS ${CMAKE_CXX_FLAGS})
set(SST_CXXCPP ${CMAKE_CXX_COMPILER})
set(SST_CXX ${CMAKE_CXX_COMPILER})
set(SST_CXXFLAGS ${CMAKE_CXX_FLAGS})
set(SST_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
set(SST_LD ${CMAKE_LINKER})
set(SST_LDFLAGS "${CMAKE_CXX_LINK_FLAGS} ${LINK_FLAGS}")
if(MPI_FOUND)
  set(SST_MPICC ${MPI_C_COMPILER})
  set(SST_MPICXX ${MPI_CXX_COMPILER})
endif(MPI_FOUND)

find_program(PYCONFIG NAMES python-config "${Python_EXECUTABLE}-config")
if(PYCONFIG)
  message(STATUS "Found PYCONFIG ${PYCONFIG}.")

  execute_process(
    COMMAND ${PYCONFIG} --ldflags
    RESULT_VARIABLE PYCONFIG_RESULT
    OUTPUT_VARIABLE SST_PYTHON_LDFLAGS
    OUTPUT_STRIP_TRAILING_WHITESPACE)

  if(PYCONFIG_RESULT AND NOT PYCONFIG_RESULT EQUAL 0)
    message(
      FATAL_ERROR
        "python-config (${PYCONFIG}) --ldflags failed with ${PYCONFIG_RESULT}")
  endif(PYCONFIG_RESULT AND NOT PYCONFIG_RESULT EQUAL 0)
else(PYCONFIG)
  message(FATAL_ERROR "Failed to find python-config")
endif(PYCONFIG)

if(Python_FOUND)
  set(SST_PYTHON_CPPFLAGS ${Python_INCLUDE_DIRS})
endif(Python_FOUND)
set(__STDC_FORMAT_MACROS ON)

if(SST_USE_MEMPOOL)
  set(USE_MEMPOOL)
endif(SST_USE_MEMPOOL)

set(SST_BUILD_WITH_CMAKE ON)
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
  COMMAND git --git-dir=${CMAKE_SOURCE_DIR}/../.git rev-parse HEAD
  RESULT_VARIABLE HASH_RESULT
  OUTPUT_VARIABLE SSTCORE_GIT_HEADSHA
  OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(
  COMMAND git --git-dir=${CMAKE_SOURCE_DIR}/../.git branch --show-current
  RESULT_VARIABLE BRANCH_RESULT
  OUTPUT_VARIABLE SSTCORE_GIT_BRANCH
  OUTPUT_STRIP_TRAILING_WHITESPACE)

check_include_file(sys/stat.h HAVE_SYS_STAT_H)
check_include_file(dlfcn.h HAVE_DLFCN_H)
check_include_file(sys/types HAVE_SYS_TYPES_H)
check_include_file(unistd.h HAVE_UNISTD_H)

check_library_exists(m sin "" HAVE_LIBM)
if(NOT HAVE_LIBM)
  message(FATAL_ERROR "Failed to detect libm")
endif(NOT HAVE_LIBM)

check_library_exists(ltdl lt_dlinit "" HAVE_LTDL_EXTERNAL)
if(NOT HAVE_LTDL_EXTERNAL)
  include(ExternalProject)
  set(LIBLTDL_INSTALL_DIRS
      "${CMAKE_BINARY_DIR}/contrib/install/"
      CACHE PATH "Path to the libldl install")
  if(NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/libltdl)
    find_program(LIBTOOLIZE NAMES libtoolize glibtoolize)
    message(STATUS "Using ${LIBTOOLIZE} as libtoolize.")
    execute_process(
      COMMAND ${LIBTOOLIZE} --quiet --ltdl=${CMAKE_CURRENT_BINARY_DIR}/libltdl
      RESULT_VARIABLE LIBTOOLIZE_RESULT)

    if(LIBTOOLIZE_RESULT AND NOT LIBTOOLIZE_RESULT EQUAL 0)
      message(
        FATAL_ERROR
          "${LIBTOOLIZE} failed to generate libltdl with result ${LIBTOOLIZE_RESULT}."
      )
    endif(LIBTOOLIZE_RESULT AND NOT LIBTOOLIZE_RESULT EQUAL 0)
  endif(NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/libltdl)

  ExternalProject_Add(
    libltdl
    SOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/libltdl
    BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/libltdl
    BUILD_BYPRODUCTS ${LIBLTDL_INSTALL_DIRS}/lib/libltdl.a
    INSTALL_DIR ${LIBLTDL_INSTALL_DIRS}
    CONFIGURE_COMMAND
      aclocal && autoconf && automake && ./configure
      --prefix=${LIBLTDL_INSTALL_DIRS} --quiet --enable-ltdl-install
    BUILD_COMMAND make --quiet
    INSTALL_COMMAND make --quiet install)
  # TODO this is probably not the right way to set these
  set(LIBLTDL_INCLUCE_DIRS
      "${LIBLTDL_INSTALL_DIRS}/include"
      CACHE PATH "Location of the installed libltdl headers")
  set(LIBLTDL_LIBRARIES
      "${LIBLTDL_INSTALL_DIRS}/lib/libltdl.a"
      CACHE FILEPATH "Path to the install libltdl library")
else(NOT HAVE_LTDL_EXTERNAL)
  add_custom_target(libltdl)
  set(LIBLTDL_INCLUCE_DIRS "")
  set(LIBLTDL_LIBRARIES "ltdl")
endif(NOT HAVE_LTDL_EXTERNAL)
message(STATUS "libltdl includes ${LIBLTDL_INCLUCE_DIRS}.")
message(STATUS "libltdl library ${LIBLTDL_LIBRARIES}.")
message(STATUS "libltdl install ${LIBLTDL_INSTALL_DIRS}.")
