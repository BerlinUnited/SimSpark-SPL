
########### next target ###############

set(filesystemstd_LIB_SRCS
   export.cpp
   filesystemstd.cpp
   filesystemstd.h
   filesystemstd_c.cpp
)

add_library(filesystemstd MODULE ${filesystemstd_LIB_SRCS})

target_link_libraries(filesystemstd ${spark_libs})

if (NOT APPLE)
   set_target_properties(filesystemstd PROPERTIES VERSION 0.0.0 SOVERSION 0)
endif (NOT APPLE)

install(TARGETS filesystemstd DESTINATION ${LIBDIR}/${CMAKE_PROJECT_NAME})
