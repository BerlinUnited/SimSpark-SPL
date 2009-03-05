########### next target ###############

set(rcssagent3d_SRCS
   main.cpp
   behavior.h
   soccerbehavior.h
   soccerbehavior.cpp
   soccerbotbehavior.h
   soccerbotbehavior.cpp
   hoap2behavior.h
   hoap2behavior.cpp
   naobehavior.h
   naobehavior.cpp
)

include_directories(${SPARK_INCLUDE_DIR} ${SALT_INCLUDE_DIR}
  ${ZEITGEIST_INCLUDE_DIR} ${OXYGEN_INCLUDE_DIR} ${KEROSIN_INCLUDE_DIR})

add_executable(rcssagent3d ${rcssagent3d_SRCS})

if (APPLE)
   target_link_libraries(rcssagent3d ${SPARK_LIBRARIES} ${SALT_LIBRARIES}
   				     ${ZEITGEIST_LIBRARIES} ${OXYGEN_LIBRARIES} ${KEROSIN_LIBRARIES}
				     ${RCSSNET3D_LIBRARIES})
else (APPLE)
   target_link_libraries(rcssagent3d ${SPARK_LIBRARIES} ${SALT_LIBRARIES}
   				     ${ZEITGEIST_LIBRARIES} ${OXYGEN_LIBRARIES} ${KEROSIN_LIBRARIES})
endif (APPLE)
  
set_target_properties(rcssagent3d PROPERTIES 
    INSTALL_RPATH "${RPATH_DIRECTORIES}")
install(TARGETS rcssagent3d DESTINATION ${BINDIR})
