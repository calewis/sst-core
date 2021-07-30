configure_file(sst_config.h.in.cmake sst_config.h)
configure_file(build_info.h.in build_info.h)

add_library(sst_core_lib  OBJECT
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
  model/python/pymacros.h
	model/python/pymodel.h
	model/python/pymodel.cc
	model/python/pymodel_link.h
	model/python/pymodel_link.cc
	model/python/pymodel_comp.h
	model/python/pymodel_comp.cc
	model/python/pymodel_unitalgebra.h
	model/python/pymodel_unitalgebra.cc
	model/python/pymodel_stat.h
	model/python/pymodel_stat.cc
	model/python/pymodel_statgroup.h
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
	impl/partitioners/linpart.h
	impl/partitioners/rrobin.cc
	impl/partitioners/rrobin.h
	impl/partitioners/selfpart.h
	impl/partitioners/selfpart.cc
	impl/partitioners/simplepart.cc
	impl/partitioners/simplepart.h
	impl/partitioners/singlepart.cc
	impl/partitioners/singlepart.h 
	impl/timevortex/timeVortexPQ.cc
	impl/timevortex/timeVortexPQ.h
  interfaces/TestEvent.cc
  interfaces/simpleMem.cc
  interfaces/stdMem.cc
  interfaces/simpleNetwork.cc
  interprocess/ipctunnel.cc
  interprocess/mmapparent.cc
  interprocess/shmparent.cc
  part/sstpart.h
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
	sync/rankSyncParallelSkip.h
	sync/rankSyncParallelSkip.cc
	sync/rankSyncSerialSkip.h
	sync/rankSyncSerialSkip.cc
	sync/syncManager.h
	sync/syncManager.cc
	sync/syncQueue.h
	sync/syncQueue.cc
	sync/threadSyncSimpleSkip.h
	sync/threadSyncSimpleSkip.cc
	sync/threadSyncQueue.h
  cputimer.cc
  iouse.cc
  simulation_impl.h
)

add_library(sst_env_lib OBJECT 
	env/envquery.h 
	env/envconfig.h 
	env/envquery.cc 
	env/envconfig.cc
  )

add_library(sst_boot_lib OBJECT 
 	bootshared.cc
 	bootshared.h
  )

add_library(sst_tiny_xml  OBJECT
	tinyxml/tinystr.cpp
	tinyxml/tinyxml.cpp
	tinyxml/tinyxmlerror.cpp
	tinyxml/tinyxmlparser.cpp
	tinyxml/tinystr.h
	tinyxml/tinyxml.h
  )

add_dependencies(sst_core_lib libltdl)
target_include_directories(sst_core_lib PUBLIC
  ${CMAKE_CURRENT_BINARY_DIR} 
  ${CMAKE_BINARY_DIR}/src
  ${CMAKE_SOURCE_DIR}/../src 
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${LIBLTDL_INCLUCE_DIRS}
  )

target_include_directories(sst_tiny_xml PUBLIC ${CMAKE_SOURCE_DIR}/../src)

target_include_directories(sst_env_lib PUBLIC 
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_BINARY_DIR}/src
  ${CMAKE_SOURCE_DIR}/../src 
  )

target_include_directories(sst_boot_lib PUBLIC 
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/../src 
  )

target_compile_definitions(sst_core_lib PUBLIC 
  __STDC_FORMAT_MACROS 
  SST_BUILDING_CORE=1)
target_compile_definitions(sst_tiny_xml PUBLIC TIXML_USE_STL)

target_link_libraries(sst_core_lib
  PUBLIC Python::Python ${LIBLTDL_LIBRARIES} m ${CMAKE_DL_LIBS}
  ) 

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
add_executable(sst_info bootsstinfo.cc)
add_executable(sstinfo.x sstinfo.cc)
add_executable(sst bootsst.cc)
add_executable(sstconfig sstconfigtool.cc)
add_executable(sst_register sstregistertool.cc)

target_link_libraries(sstsim.x PRIVATE sst_core_lib sst_env_lib)
target_link_libraries(sstinfo.x PRIVATE sst_core_lib sst_tiny_xml sst_env_lib)
target_link_libraries(sst_info PRIVATE sst_env_lib sst_boot_lib)
target_link_libraries(sst PRIVATE sst_boot_lib sst_env_lib)
target_link_libraries(sstconfig PRIVATE sst_env_lib)
target_link_libraries(sst_register PRIVATE sst_env_lib)

if(NOT APPLE)
target_link_libraries(sstinfo.x PRIVATE rt)
target_link_libraries(sstsim.x PRIVATE rt)
endif(NOT APPLE)
