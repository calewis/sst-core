add_library(
  coreTestElement SHARED
  coreTest_Component.cc
  coreTest_ClockerComponent.cc
  coreTest_DistribComponent.cc
  coreTest_RNGComponent.cc
  coreTest_StatisticsComponent.cc
  coreTest_Links.cc
  coreTest_MessageGeneratorComponent.cc
  coreTest_SharedObjectComponent.cc
  coreTest_SubComponent.cc
  coreTest_Module.cc
  coreTest_ParamComponent.cc)

target_include_directories(
  coreTestElement PUBLIC ${CMAKE_SOURCE_DIR}/../src/
                         ${CMAKE_CURRENT_BINARY_DIR}/..)
# target_link_options(coreTestElement PRIVATE LINKER:-module,-avoid-version)
install(TARGETS coreTestElement DESTINATION lib/sstcore)
