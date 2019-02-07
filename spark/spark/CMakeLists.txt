########### next target ###############

set(spark_LIB_SRCS
   spark.cpp
   spark.h
)

include_directories(${CMAKE_SOURCE_DIR}/lib ${CMAKE_SOURCE_DIR}/utility ${FREETYPE_INCLUDE_DIRS}
  ${IL_INCLUDE_DIR})

add_library(spark ${spark_LIB_SRCS})

target_link_libraries(spark salt zeitgeist oxygen kerosin)

set_target_properties(spark PROPERTIES VERSION ${SPARK_VERSION}
    SOVERSION ${SPARK_SO_VERSION} DEBUG_POSTFIX _debug)
install(TARGETS spark DESTINATION ${LIBDIR}/${CMAKE_PROJECT_NAME})

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/spark-config.in
    ${CMAKE_CURRENT_BINARY_DIR}/spark-config)

########### install files ###############

install(DIRECTORY . DESTINATION ${DATADIR}/${CMAKE_PROJECT_NAME}
        FILES_MATCHING PATTERN "*.rb" PATTERN ".svn" EXCLUDE)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/spark-config DESTINATION ${BINDIR}
          PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
          GROUP_EXECUTE GROUP_READ WORLD_READ WORLD_EXECUTE)
install(FILES spark.h DESTINATION ${INCLUDEDIR}/${CMAKE_PROJECT_NAME}/spark)
