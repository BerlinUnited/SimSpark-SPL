
########### next target ###############

set(perfectvisionperceptor_LIB_SRCS
   export.cpp
   perfectvisionperceptor.h
   perfectvisionperceptor.cpp
   perfectvisionperceptor_c.cpp
)

add_library(perfectvisionperceptor MODULE ${perfectvisionperceptor_LIB_SRCS})

target_link_libraries(perfectvisionperceptor)

set_target_properties(perfectvisionperceptor PROPERTIES VERSION 0.0.0 SOVERSION 0)
install(TARGETS perfectvisionperceptor DESTINATION ${LIBDIR}/${CMAKE_PROJECT_NAME})

