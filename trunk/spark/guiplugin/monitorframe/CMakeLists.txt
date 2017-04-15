########### target files ###############
set(monitorframe_QOBJECT_HEADERS
	monitorframe.h
	monitorframe_sparkglwidget.h
)

set(monitorframe_HEADERS
	monitorframe_sparkglrender.h
	monitorframe_glconstants.h
	export.h
)

set(monitorframe_SOURCES
	monitorframe.cpp
	monitorframe_sparkglrender.cpp
	monitorframe_sparkglwidget.cpp
	monitorframe_glconstants.cpp
)

set(monitorframe_FORMS
	monitorframe.ui
)

#set(monitorframe_RESOURCES)

########### qt properties ###############

#Additional used libraries
set(QT_USE_QTOPENGL TRUE)
set(QT_USE_QTXML TRUE)

#Include qt properties
include(${QT_USE_FILE})	
add_definitions(${QT_DEFINITIONS})

########### create library ###############

#Meta object compiler wrapping of header files, forms and resources
QT4_WRAP_CPP(monitorframe_QOBJECT_HEADERS_MOC ${monitorframe_QOBJECT_HEADERS})
QT4_WRAP_UI(monitorframe_FORMS_HEADERS ${monitorframe_FORMS})
#QT4_ADD_RESOURCES(monitorframe_RESOURCES_RCC ${monitorframe_RESOURCES})

include_directories(
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/carbon
  ${FREETYPE_INCLUDE_DIRS}
  ${IL_INCLUDE_DIR}
  ${OPENGL_INCLUDE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_BINARY_DIR}/carbon)

add_library(
  monitorframe MODULE 
  ${monitorframe_SOURCES} 
  ${monitorframe_QOBJECT_HEADERS}
  ${monitorframe_QOBJECT_HEADERS_MOC}
  ${monitorframe_HEADERS}
  ${monitorframe_FORMS_HEADERS})
  #${monitorframe_RESOURCES_RCC})

target_link_libraries(
  monitorframe
  ${OPENGL_LIBRARIES} 
  ${QT_LIBRARIES} 
  ${spark_gui_libs})

if (NOT APPLE)
   set_target_properties(monitorframe PROPERTIES VERSION 1.0.0 SOVERSION 1)
endif (NOT APPLE)

install(TARGETS monitorframe DESTINATION ${CARBON_PLUGIN_LIB_DIR})

########### install files ###############

install(DIRECTORY ./ DESTINATION ${CARBON_PLUGIN_INCLUDE_DIR}/monitorframe
          FILES_MATCHING PATTERN "*.h" PATTERN ".svn" EXCLUDE)
