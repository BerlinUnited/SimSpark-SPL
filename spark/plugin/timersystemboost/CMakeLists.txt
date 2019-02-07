########### next target ###############

set(timersystemboost_LIB_SRCS
   export.cpp
   timersystemboost.cpp
   timersystemboost_c.cpp
   timersystemboost.h
)

add_library(timersystemboost MODULE ${timersystemboost_LIB_SRCS})

target_link_libraries(timersystemboost ${spark_libs})

if (NOT APPLE)
   set_target_properties(timersystemboost PROPERTIES VERSION 0.0.0 SOVERSION 0)
endif (NOT APPLE)

install(TARGETS timersystemboost DESTINATION ${LIBDIR}/${CMAKE_PROJECT_NAME})
