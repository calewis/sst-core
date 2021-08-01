configure_file(sst_config.h.in.cmake ${CMAKE_CURRENT_BINARY_DIR}/sst_config.h)
configure_file(build_info.h.in ${CMAKE_CURRENT_BINARY_DIR}/build_info.h)

add_subdirectory(testingframework)
add_subdirectory(testElements)

add_library(
  sst_core_lib OBJECT
  action.cc
  clock.cc
  baseComponent.cc
  component.cc
  componentExtension.cc
  componentInfo.cc
  config.cc
  configGraph.cc
  cfgoutput/pythonConfigOutput.cc
  cfgoutput/dotConfigOutput.cc
  cfgoutput/xmlConfigOutput.cc
  cfgoutput/jsonConfigOutput.cc
  eli/elibase.cc
  eli/elementinfo.cc
  elemLoader.cc
  event.cc
  exit.cc
  factory.cc
  heartbeat.cc
  initQueue.cc
  link.cc
  memuse.cc
  model/sstmodel.cc
  model/element_python.cc
  model/python/pymodel.h
  model/python/pymodel.cc
  model/python/pymodel_link.cc
  model/python/pymodel_comp.cc
  model/python/pymodel_unitalgebra.cc
  model/python/pymodel_stat.cc
  model/python/pymodel_statgroup.cc
  oneshot.cc
  output.cc
  params.cc
  pollingLinkQueue.cc
  simulation.cc
  subcomponent.cc
  sharedRegion.cc
  timeLord.cc
  uninitializedQueue.cc
  unitAlgebra.cc
  module.cc
  sstpart.cc
  timeVortex.cc
  serialization/serializable.cc
  serialization/serialize_serializable.cc
  serialization/serializer.cc
  serialization/statics.cc
  impl/partitioners/linpart.cc
  impl/partitioners/rrobin.cc
  impl/partitioners/selfpart.cc
  impl/partitioners/simplepart.cc
  impl/partitioners/singlepart.cc
  impl/timevortex/timeVortexPQ.cc
  interfaces/TestEvent.cc
  interfaces/simpleMem.cc
  interfaces/stdMem.cc
  interfaces/simpleNetwork.cc
  interprocess/ipctunnel.cc
  interprocess/mmapparent.cc
  interprocess/shmparent.cc
  rng/marsaglia.cc
  rng/mersenne.cc
  rng/xorshift.cc
  shared/sharedObject.cc
  statapi/statengine.cc
  statapi/statgroup.cc
  statapi/statoutput.cc
  statapi/statfieldinfo.cc
  statapi/statoutputconsole.cc
  statapi/statoutputtxt.cc
  statapi/statoutputcsv.cc
  statapi/statoutputjson.cc
  statapi/statbase.cc
  sync/rankSyncParallelSkip.cc
  sync/rankSyncSerialSkip.cc
  sync/syncManager.cc
  sync/syncQueue.cc
  sync/threadSyncSimpleSkip.cc
  cputimer.cc
  iouse.cc)

add_library(sst_env_lib OBJECT env/envquery.cc env/envconfig.cc)
add_library(sst_boot_lib OBJECT bootshared.cc)

add_library(
  sst_tiny_xml OBJECT tinyxml/tinystr.cpp tinyxml/tinyxml.cpp
                      tinyxml/tinyxmlerror.cpp tinyxml/tinyxmlparser.cpp)

add_dependencies(sst_core_lib libltdl)
target_include_directories(
  sst_core_lib
  PUBLIC ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_BINARY_DIR}/src
         ${CMAKE_SOURCE_DIR}/../src ${CMAKE_CURRENT_SOURCE_DIR}
         ${LIBLTDL_INCLUCE_DIRS})

target_include_directories(sst_tiny_xml PUBLIC ${CMAKE_SOURCE_DIR}/../src)

target_include_directories(
  sst_env_lib PUBLIC ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_BINARY_DIR}/src
                     ${CMAKE_SOURCE_DIR}/../src)

target_include_directories(sst_boot_lib PUBLIC ${CMAKE_CURRENT_BINARY_DIR}
                                               ${CMAKE_SOURCE_DIR}/../src)

target_compile_definitions(sst_core_lib PUBLIC __STDC_FORMAT_MACROS
                                               SST_BUILDING_CORE=1)

if(SST_USE_MEMPOOL)
  target_compile_definitions(sst_core_lib PUBLIC USE_MEMPOOL=1)
endif(SST_USE_MEMPOOL)

target_compile_definitions(sst_tiny_xml PUBLIC TIXML_USE_STL)

target_link_libraries(sst_core_lib PUBLIC Python::Python ${LIBLTDL_LIBRARIES} m
                                          ${CMAKE_DL_LIBS})

if(MPI_FOUND)
  target_link_libraries(sst_core_lib PUBLIC MPI::MPI_C)
endif(MPI_FOUND)

if(ZLIB_FOUND)
  target_link_libraries(sst_core_lib PUBLIC ZLIB::ZLIB)
endif(ZLIB_FOUND)

if(HDF5_FOUND)
  target_sources(sst_core_lib statapi/statoutputhdf5.cc)
  target_link_libraries(sst_core_lib PUBLIC HDF5::HDF5)
endif(HDF5_FOUND)

add_executable(sstsim.x main.cc)
add_executable(sst-info bootsstinfo.cc)
add_executable(sstinfo.x sstinfo.cc)
add_executable(sst bootsst.cc)
add_executable(sst-config sstconfigtool.cc)
add_executable(sst-register sstregistertool.cc)

set_target_properties(sstinfo.x sstsim.x PROPERTIES ENABLE_EXPORTS 1)

target_link_libraries(sstsim.x PRIVATE sst_core_lib sst_env_lib)
target_link_libraries(sstinfo.x PRIVATE sst_core_lib sst_tiny_xml sst_env_lib)
target_link_libraries(sst-info PRIVATE sst_env_lib sst_boot_lib)
target_link_libraries(sst PRIVATE sst_boot_lib sst_env_lib)
target_link_libraries(sst-config PRIVATE sst_env_lib)
target_link_libraries(sst-register PRIVATE sst_env_lib)

if(NOT APPLE)
  target_link_libraries(sstinfo.x PRIVATE rt)
  target_link_libraries(sstsim.x PRIVATE rt)
endif(NOT APPLE)

install(TARGETS sst)
install(TARGETS sstsim.x DESTINATION libexec)
install(TARGETS sst-info)
install(TARGETS sstinfo.x DESTINATION libexec)
install(TARGETS sst-register)
install(TARGETS sst-config)

# We may need to filter this list if it over installs.
install(
  DIRECTORY "${CMAKE_SOURCE_DIR}/../src/"
  DESTINATION "include"
  FILES_MATCHING
  PATTERN "*.h")
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/sst_config.h
        DESTINATION include/sst/core/)
