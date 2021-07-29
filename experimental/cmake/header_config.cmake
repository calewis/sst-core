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

execute_process(
  COMMAND libtoolize --quiet --ltdl=${CMAKE_SOURCE_DIR}../src/sst/core/libltdl
  RESULT_VARIABLE LTDL_RESULT
  )

if(LTDL_RESULT AND NOT LTDL_RESULT EQUAL 0)
  if(NOT APPLE)
    if(LTDL_RESULT AND NOT LTDL_RESULT EQUAL 0)
      message(FATAL_ERROR "glibtoolize failed with: ${APPLE_LTDL_RESULT}")
    endif(LTDL_RESULT AND NOT LTDL_RESULT EQUAL 0)

  else(NOT APPLE)
    execute_process(
      COMMAND glibtoolize --quiet --ltdl=${CMAKE_SOURCE_DIR}../src/sst/core/libltdl
      RESULT_VARIABLE APPLE_LTDL_RESULT
      )

    if(APPLE_LTDL_RESULT AND NOT APPLE_LTDL_RESULT EQUAL 0)
      message(FATAL_ERROR "glibtoolize failed with: ${APPLE_LTDL_RESULT}")
    endif(APPLE_LTDL_RESULT AND NOT APPLE_LTDL_RESULT EQUAL 0)
  endif(NOT APPLE)
endif(LTDL_RESULT AND NOT LTDL_RESULT EQUAL 0)
