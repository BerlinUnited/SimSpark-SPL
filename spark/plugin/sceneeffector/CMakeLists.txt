
########### next target ###############

set(sceneeffector_LIB_SRCS
   export.cpp
   sceneeffector.cpp
   sceneeffector.h
   sceneeffector_c.cpp
   sceneaction.h
)

add_library(sceneeffector MODULE ${sceneeffector_LIB_SRCS})

target_link_libraries(sceneeffector ${spark_libs})

set_target_properties(sceneeffector PROPERTIES VERSION 0.0.0 SOVERSION 0)
install(TARGETS sceneeffector DESTINATION ${LIBDIR}/${CMAKE_PROJECT_NAME})
