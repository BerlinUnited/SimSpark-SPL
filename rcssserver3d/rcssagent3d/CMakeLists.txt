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

target_link_libraries(rcssagent3d
    ${RCSSNET3D_LIBRARIES} ${NET_LIBS}
    debug ${SPARK_LIBRARY_DEBUG}
    debug ${SALT_LIBRARY_DEBUG}
    debug ${ZEITGEIST_LIBRARY_DEBUG}
    debug ${OXYGEN_LIBRARY_DEBUG}
    debug ${KEROSIN_LIBRARY_DEBUG}
    optimized ${SPARK_LIBRARY_RELEASE}
    optimized ${SALT_LIBRARY_RELEASE}
    optimized ${ZEITGEIST_LIBRARY_RELEASE}
    optimized ${OXYGEN_LIBRARY_RELEASE}
    optimized ${KEROSIN_LIBRARY_RELEASE}
)

set_target_properties(rcssagent3d PROPERTIES
    INSTALL_RPATH "${RPATH_DIRECTORIES}")
install(TARGETS rcssagent3d DESTINATION ${BINDIR})
