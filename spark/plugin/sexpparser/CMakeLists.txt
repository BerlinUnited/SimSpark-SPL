########### next target ###############

set(sexpparser_LIB_SRCS
   export.cpp
   sexpparser.cpp
   sexpparser_c.cpp
   sexpparser.h
)

add_library(sexpparser MODULE ${sexpparser_LIB_SRCS})

target_link_libraries(sexpparser sexp ${spark_libs})

if (NOT APPLE)
   set_target_properties(sexpparser PROPERTIES VERSION 0.0.0 SOVERSION 0)
endif (NOT APPLE)

install(TARGETS sexpparser DESTINATION ${LIBDIR}/${CMAKE_PROJECT_NAME})
