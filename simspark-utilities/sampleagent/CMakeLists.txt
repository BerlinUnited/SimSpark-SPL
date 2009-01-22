########### next target ###############

set(sampleagent_SRCS
   main.cpp
   behavior.h
   carbehavior.cpp
   carbehavior.h
   leggedspherebehavior.h
   leggedspherebehavior.cpp
)

include_directories(${SPARK_INCLUDE_DIR} ${Boost_INCLUDE_DIR})
add_executable(sampleagent ${sampleagent_SRCS})

target_link_libraries(sampleagent ${SPARK_LIBRARIES} ${SALT_LIBRARIES}
  ${ZEITGEIST_LIBRARIES} ${OXYGEN_LIBRARIES} ${KEROSIN_LIBRARIES})

set_target_properties(sampleagent PROPERTIES
    INSTALL_RPATH "${RPATH_DIRECTORIES}")

install(TARGETS sampleagent DESTINATION ${BINDIR})
