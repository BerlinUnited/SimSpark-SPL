
########### next target ###############

set(forceeffector_LIB_SRCS
   export.cpp
   forceeffector.cpp
   forceeffector.h
   forceeffector_c.cpp
   forceaction.h
)

add_library(forceeffector MODULE ${forceeffector_LIB_SRCS})

target_link_libraries(forceeffector ${spark_libs})

set_target_properties(forceeffector PROPERTIES VERSION 0.0.0 SOVERSION 0)
install(TARGETS forceeffector DESTINATION ${LIBDIR}/${CMAKE_PROJECT_NAME})
