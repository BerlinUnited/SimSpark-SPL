
########### next target ###############

set(objimporter_LIB_SRCS
   export.cpp
   objimporter.cpp
   objimporter_c.cpp
   objimporter.h
)

add_library(objimporter MODULE ${objimporter_LIB_SRCS})

target_link_libraries(objimporter ${spark_libs})

if (NOT APPLE)
   set_target_properties(objimporter PROPERTIES VERSION 0.0.0 SOVERSION 0)
endif (NOT APPLE)

install(TARGETS objimporter DESTINATION ${LIBDIR}/${CMAKE_PROJECT_NAME})
