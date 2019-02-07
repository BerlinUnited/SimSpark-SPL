
########### next target ###############

set(batteryperceptor_LIB_SRCS
   export.cpp
   batteryperceptor.h
   batteryperceptor.cpp
   batteryperceptor_c.cpp
)

add_library(batteryperceptor MODULE ${batteryperceptor_LIB_SRCS})

target_link_libraries(batteryperceptor ${spark_libs} ${Boost_LIBRARIES})

if (NOT APPLE)
   set_target_properties(batteryperceptor PROPERTIES VERSION 0.0.0 SOVERSION 0)
endif (NOT APPLE)

if (NOT APPLE)
   install(TARGETS batteryperceptor DESTINATION ${LIBDIR}/${CMAKE_PROJECT_NAME})
endif (NOT APPLE)
