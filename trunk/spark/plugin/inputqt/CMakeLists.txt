########### target files ###############
set(inputqt_HEADERS
	inputqt.h
	inputsystemqt.h
)

set(inputqt_SOURCES
	inputqt.cpp
	inputsystemqt.cpp
	inputsystemqt_c.cpp
	export.cpp
)

########### qt properties ###############

#Additional used libraries

#Include qt properties
include(${QT_USE_FILE})	
add_definitions(${QT_DEFINITIONS})

########### create library ###############
  
add_library(inputqt MODULE ${inputqt_SOURCES} ${inputqt_HEADERS})

target_link_libraries(inputqt ${QT_LIBRARIES} ${spark_libs})

if (NOT APPLE)
   set_target_properties(inputqt PROPERTIES VERSION 1.0.0 SOVERSION 0)
endif (NOT APPLE)

install(TARGETS inputqt DESTINATION ${LIBDIR}/${CMAKE_PROJECT_NAME})

