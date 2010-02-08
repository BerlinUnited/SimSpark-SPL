
set(accelerometer_LIB_SRCS
   export.cpp
   accelerometer.h
   accelerometer.cpp
   accelerometer_c.cpp
)

add_library(accelerometer MODULE ${accelerometer_LIB_SRCS})

target_link_libraries(accelerometer ${spark_libs} ${Boost_LIBRARIES})

if (NOT APPLE)
   set_target_properties(accelerometer PROPERTIES VERSION 0.0.0 SOVERSION 0)
endif (NOT APPLE)

install(TARGETS accelerometer DESTINATION ${LIBDIR}/${CMAKE_PROJECT_NAME})
