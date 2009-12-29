
########### next target ###############

set(agentsynceffector_LIB_SRCS
   export.cpp
   agentsynceffector.cpp
   agentsynceffector.h
   agentsynceffector_c.cpp
)

add_library(agentsynceffector MODULE ${agentsynceffector_LIB_SRCS})

target_link_libraries(agentsynceffector ${spark_libs})

if (NOT APPLE)
   set_target_properties(agentsynceffector PROPERTIES VERSION 0.0.0 SOVERSION 0)
endif (NOT APPLE)

install(TARGETS agentsynceffector DESTINATION ${LIBDIR}/${CMAKE_PROJECT_NAME})
