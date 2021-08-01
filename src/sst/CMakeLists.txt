configure_file(sstsimulator.conf.in ${CMAKE_BINARY_DIR}/etc/sst/sstsimulator.conf)
install(FILES ${CMAKE_BINARY_DIR}/etc/sst/sstsimulator.conf DESTINATION etc/sst)
add_subdirectory(core)
