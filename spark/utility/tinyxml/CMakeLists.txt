########### next target ###############

set(tinyxml_ex_LIB_SRCS
   tinystr.cpp
   tinyxml.cpp
   tinyxmlerror.cpp
   tinyxmlparser.cpp
   xmlfunctions.cpp
   tinystr.h
   tinyxml.h
   xmlfunctions.h
)

add_library(tinyxml_ex ${tinyxml_ex_LIB_SRCS})

target_link_libraries(tinyxml_ex)

set_target_properties(tinyxml_ex PROPERTIES VERSION 0.0.0 SOVERSION 0)
install(TARGETS tinyxml_ex DESTINATION ${LIBDIR}/${CMAKE_PROJECT_NAME})
