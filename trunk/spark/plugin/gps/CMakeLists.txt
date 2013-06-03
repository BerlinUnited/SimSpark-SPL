
########### next target ###############

set(gps_LIB_SRCS
   export.cpp
   gps.h
   gps.cpp
   gps_c.cpp
)

add_library(gps MODULE ${gps_LIB_SRCS})

target_link_libraries(gps ${spark_libs})

if (NOT APPLE)
   set_target_properties(gps PROPERTIES VERSION 0.0.0 SOVERSION 0)
endif (NOT APPLE)

install(TARGETS gps DESTINATION ${LIBDIR}/${CMAKE_PROJECT_NAME})
