
########### next target ###############

set(imageperceptor_LIB_SRCS
   export.cpp
   imageperceptor.h
   imageperceptor.cpp
   imageperceptor_c.cpp
)

if(WIN32)
	include_directories(${CMAKE_SOURCE_DIR}/windows/include) 
endif(WIN32)

add_library(imageperceptor MODULE ${imageperceptor_LIB_SRCS})

target_link_libraries(imageperceptor ${spark_libs})

set_target_properties(imageperceptor PROPERTIES VERSION 0.0.0 SOVERSION 0)
if (NOT APPLE)
   install(TARGETS imageperceptor DESTINATION ${LIBDIR}/${CMAKE_PROJECT_NAME})
endif (NOT APPLE)
