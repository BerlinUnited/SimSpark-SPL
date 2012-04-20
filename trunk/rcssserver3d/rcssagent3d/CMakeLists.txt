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

add_executable(rcssagent3d ${rcssagent3d_SRCS})

if (WIN32)
 set(NET_LIBS ws2_32.lib)
endif (WIN32)

target_link_libraries(rcssagent3d ${SPARK_LIBRARIES} ${SALT_LIBRARIES}
   	${ZEITGEIST_LIBRARIES} ${OXYGEN_LIBRARIES} ${KEROSIN_LIBRARIES}
    ${RCSSNET3D_LIBRARIES} ${NET_LIBS})
  
set_target_properties(rcssagent3d PROPERTIES 
    INSTALL_RPATH "${RPATH_DIRECTORIES}")
install(TARGETS rcssagent3d DESTINATION ${BINDIR})
