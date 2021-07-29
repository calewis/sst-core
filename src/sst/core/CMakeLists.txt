configure_file(sst_config.h.in.cmake sst_config.h)
configure_file(build_info.h.in build_info.h)

#
# 
#

#
# SSTSIM
#
add_executable(sstsim 
  main.cc 
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
  env/envquery.cc
  env/envconfig.cc
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

#add_executable(sstinfo
# 	bootsstinfo.cc
# 	bootshared.cc
# 	bootshared.h
# 	env/envquery.h
# 	env/envconfig.h
# 	env/envquery.cc
# 	env/envconfig.cc
#   )
# 
# add_executable(sst
# 	bootsst.cc
# 	bootshared.cc
# 	bootshared.h
# 	env/envquery.h
# 	env/envconfig.h
# 	env/envquery.cc
# 	env/envconfig.cc
#   )
# 
# add_executable(sstconfig
# 	sstconfigtool.cc
# 	env/envquery.h
# 	env/envconfig.h
# 	env/envquery.cc
# 	env/envconfig.cc
#   )

#
# SSTSIM
#
add_dependencies(sstsim libltdl)
target_include_directories(sstsim PRIVATE 
  ${CMAKE_CURRENT_BINARY_DIR} 
  ${CMAKE_BINARY_DIR}/src
  ${CMAKE_SOURCE_DIR}/../src 
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${LIBLTDL_INCLUCE_DIRS}
  )
target_compile_definitions(sstsim
  PRIVATE -DTIXML_USE_STL -DSST_BUILDING_CORE=1
  )
#target_link_options(sstsim BEFORE PRIVATE "-export-dynamic")
target_link_libraries(sstsim 
  PUBLIC Python::Python ${LIBLTDL_LIBRARIES} m
  ) 

if(MPI_FOUND)
  target_link_libraries(sstsim PRIVATE MPI::MPI_C) 
endif(MPI_FOUND)

if(ZLIB_FOUND)
  target_link_libraries(sstsim PRIVATE ZLIB::ZLIB) 
endif(ZLIB_FOUND)

if(ZOLTAN_FOUND)
  target_sources(sstsim 
    impl/partitioners/zoltpart.cc 
    impl/partitioners/zoltpart.h
    )
endif(ZOLTAN_FOUND)

if(HDF5_FOUND)
  target_sources(sstsim statapi/statoutputhdf5.cc)
  target_link_libraries(sstsim PRIVATE HDF5::HDF5) 
endif(HDF5_FOUND)



