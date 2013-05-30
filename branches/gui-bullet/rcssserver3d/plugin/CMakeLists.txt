if (WIN32 OR APPLE)
   set(spark_release_libs 
      optimized ${SALT_LIBRARY_RELEASE} 
      optimized ${OXYGEN_LIBRARY_RELEASE} 
      optimized ${ZEITGEIST_LIBRARY_RELEASE}
      optimized ${KEROSIN_LIBRARY_RELEASE})
   set(spark_debug_libs 
      debug ${SALT_LIBRARY_DEBUG} 
      debug ${OXYGEN_LIBRARY_DEBUG} 
      debug ${ZEITGEIST_LIBRARY_DEBUG}
      debug ${KEROSIN_LIBRARY_DEBUG})
   set(spark_libs ${RCSSNET3D_LIBRARY})
endif (WIN32 OR APPLE)

add_subdirectory(soccer)
add_subdirectory(soccermonitor)
