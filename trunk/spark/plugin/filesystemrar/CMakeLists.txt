
########### next target ###############

set(filesystemrar_LIB_SRCS
   export.cpp
   filesystemrar.cpp
   filesystemrar.h
   filesystemrar_c.cpp
)

add_library(filesystemrar MODULE ${filesystemrar_LIB_SRCS})

target_link_libraries(filesystemrar ${spark_libs})

set_target_properties(filesystemrar PROPERTIES VERSION 0.0.0 SOVERSION 0)
install(TARGETS filesystemrar DESTINATION ${LIBDIR}/${CMAKE_PROJECT_NAME})
