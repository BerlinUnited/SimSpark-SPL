
########### next target ###############

set(collisionperceptor_LIB_SRCS
   export.cpp
   collisionperceptor.h
   collisionperceptor.cpp
   collisionperceptor_c.cpp
   perceptorhandler.h
   perceptorhandler.cpp
   perceptorhandler_c.cpp
   touchperceptor.h
   touchperceptor.cpp
   touchperceptor_c.cpp
   touchperceptorhandler.h
   touchperceptorhandler.cpp
   touchperceptorhandler_c.cpp
   forceresistanceperceptor.h
   forceresistanceperceptor.cpp
   forceresistanceperceptor_c.cpp
)

add_library(collisionperceptor MODULE ${collisionperceptor_LIB_SRCS})

target_link_libraries(collisionperceptor ${spark_libs})

if (NOT APPLE)
   set_target_properties(collisionperceptor PROPERTIES VERSION 0.0.0 SOVERSION 0)
endif (NOT APPLE)

install(TARGETS collisionperceptor DESTINATION ${LIBDIR}/${CMAKE_PROJECT_NAME})
