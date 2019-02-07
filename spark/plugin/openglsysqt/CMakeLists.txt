########### target files ###############
set(openglsystemqt_HEADERS
	openglsystemqt.h
)

set(openglsystemqt_SOURCES
	openglsystemqt_c.cpp
	openglsystemqt.cpp
	export.cpp
)

########### qt properties ###############

#Additional used libraries
set(QT_USE_QTOPENGL TRUE)

#Include qt properties
include(${QT_USE_FILE})	
add_definitions(${QT_DEFINITIONS})

########### create library ###############
  
add_library(openglsystemqt MODULE ${openglsystemqt_SOURCES} ${inputqt_HEADERS})

target_link_libraries(openglsystemqt ${QT_LIBRARIES} ${OPENGL_LIBRARIES} ${spark_libs})

if (NOT APPLE)
   set_target_properties(openglsystemqt PROPERTIES VERSION 1.0.0 SOVERSION 0)
endif (NOT APPLE)

install(TARGETS openglsystemqt DESTINATION ${LIBDIR}/${CMAKE_PROJECT_NAME})