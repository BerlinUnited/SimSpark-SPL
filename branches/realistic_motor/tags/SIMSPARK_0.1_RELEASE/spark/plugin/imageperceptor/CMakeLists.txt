
########### next target ###############

set(imageperceptor_LIB_SRCS
   export.cpp
   imageperceptor.h
   imageperceptor.cpp
   imageperceptor_c.cpp
)

add_library(imageperceptor MODULE ${imageperceptor_LIB_SRCS})

target_link_libraries(imageperceptor)

set_target_properties(imageperceptor PROPERTIES VERSION 0.0.0 SOVERSION 0)
install(TARGETS imageperceptor DESTINATION ${LIBDIR}/${CMAKE_PROJECT_NAME})
