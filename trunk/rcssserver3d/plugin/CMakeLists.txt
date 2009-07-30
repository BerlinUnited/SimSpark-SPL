if (WIN32 OR APPLE)
   set(spark_libs ${SALT_LIBRARIES} ${OXYGEN_LIBRARIES} ${ZEITGEIST_LIBRARIES}
    ${KEROSIN_LIBRARIES} ${RCSSNET3D_LIBRARY})
endif (WIN32 OR APPLE)

add_subdirectory(soccer)
add_subdirectory(soccermonitor)
