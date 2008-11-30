########### next target ###############

set(spark_LIB_SRCS
   spark.cpp
)

include_directories(${CMAKE_SOURCE_DIR}/lib ${CMAKE_SOURCE_DIR}/utility
    ${FREETYPE_INCLUDE_DIRS})

add_library(spark SHARED ${spark_LIB_SRCS})

target_link_libraries(spark)

set_target_properties(spark PROPERTIES VERSION ${SPARK_VERSION}
    SOVERSION ${SPARK_SO_VERSION} DEBUG_POSTFIX _debug)
install(TARGETS spark DESTINATION ${LIBDIR}/${CMAKE_PROJECT_NAME})

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/spark-config.in
    ${CMAKE_CURRENT_BINARY_DIR}/spark-config)

########### install files ###############

install(FILES  spark.rb DESTINATION ${DATADIR}/${CMAKE_PROJECT_NAME})
install(FILES   ${CMAKE_CURRENT_BINARY_DIR}/spark-config DESTINATION ${BINDIR}
          PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
          GROUP_EXECUTE GROUP_READ WORLD_READ WORLD_EXECUTE)
install(FILES  spark.h DESTINATION ${INCLUDEDIR}/${CMAKE_PROJECT_NAME}/spark)



#original Makefile.am contents follow:

#libspark_debug_la_CXXFLAGS = -O -g -W -Wall
#libspark_la_LIBADD = @FREETYPE_LIBADD@ @SDL_LIBADD@ @IL_LIBADD@ @GL_LIBADD@
#bin_SCRIPTS = spark-config
#
#AM_CPPFLAGS = -I${top_srcdir}/lib -I${top_srcdir}/utility \
#              @FREETYPE_CPPFLAGS@ @RUBY_CPPFLAGS@ @SDL_CPPFLAGS@
#
#
#nobase_libpkginclude_HEADERS = \
#spark.h
