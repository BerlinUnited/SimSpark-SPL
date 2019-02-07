########### next target ###############

set(sampleagent_SRCS
   main.cpp
   behavior.h
   carbehavior.cpp
   carbehavior.h
   leggedspherebehavior.h
   leggedspherebehavior.cpp
)

add_executable(sampleagent ${sampleagent_SRCS})

target_link_libraries(sampleagent ${SPARK_LIBRARIES} ${SALT_LIBRARIES}
  ${ZEITGEIST_LIBRARIES} ${OXYGEN_LIBRARIES} ${KEROSIN_LIBRARIES} ${RCSSNET3D_LIBRARIES} ${NET_LIBS})

set_target_properties(sampleagent PROPERTIES
    INSTALL_RPATH "${RPATH_DIRECTORIES}")

install(TARGETS sampleagent DESTINATION ${BINDIR})
