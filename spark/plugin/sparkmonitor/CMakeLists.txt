########### next target ###############

set(sparkmonitor_LIB_SRCS
   export.cpp
   sparkmonitor.cpp
   sparkmonitor_c.cpp
   sparkmonitor.h
   sparkmonitorclient.h
   sparkmonitorclient.cpp
   sparkmonitorclient_c.cpp
   sparkmonitorlogfileserver.h
   sparkmonitorlogfileserver.cpp
   sparkmonitorlogfileserver_c.cpp
)

add_library(sparkmonitor MODULE ${sparkmonitor_LIB_SRCS})

target_link_libraries(sparkmonitor sexp ${spark_libs})

if (NOT APPLE)
   set_target_properties(sparkmonitor PROPERTIES VERSION 0.0.0 SOVERSION 0)
endif (NOT APPLE)

install(TARGETS sparkmonitor DESTINATION ${LIBDIR}/${CMAKE_PROJECT_NAME})
