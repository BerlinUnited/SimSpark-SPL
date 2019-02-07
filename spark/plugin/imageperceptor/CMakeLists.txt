
########### next target ###############

set(imageperceptor_LIB_SRCS
   export.cpp
   imageperceptor.h
   imageperceptor.cpp
   imageperceptor_c.cpp
   imagerender.h
   imagerender.cpp
   imagerender_c.cpp
)

add_library(imageperceptor MODULE ${imageperceptor_LIB_SRCS})

target_link_libraries(imageperceptor ${spark_libs} ${Boost_LIBRARIES} b64)

if (NOT APPLE)
   set_target_properties(imageperceptor PROPERTIES VERSION 0.0.0 SOVERSION 0)
endif (NOT APPLE)

if (NOT APPLE)
   install(TARGETS imageperceptor DESTINATION ${LIBDIR}/${CMAKE_PROJECT_NAME})
endif (NOT APPLE)
