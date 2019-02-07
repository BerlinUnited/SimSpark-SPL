########### target files ###############
set(logframe_QOBJECT_HEADERS
	logframe.h
	logframe_logsettings.h
)

set(logframe_HEADERS
	export.h
)

set(logframe_SOURCES
	logframe.cpp
	logframe_logsettings.cpp
)

set(logframe_FORMS
	logframe.ui
	logframe_logsettings.ui
)

#set(logframe_RESOURCES)

########### qt properties ###############

#Additional used libraries
set(QT_USE_QTOPENGL TRUE)
set(QT_USE_QTXML TRUE)

#Include qt properties
include(${QT_USE_FILE})	
add_definitions(${QT_DEFINITIONS})

########### create library ###############

#Meta object compiler wrapping of header files, forms and resources
QT4_WRAP_CPP(logframe_QOBJECT_HEADERS_MOC ${logframe_QOBJECT_HEADERS})
QT4_WRAP_UI(logframe_FORMS_HEADERS ${logframe_FORMS})
#QT4_ADD_RESOURCES(logframe_RESOURCES_RCC ${logframe_RESOURCES})

include_directories(
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/carbon
  ${FREETYPE_INCLUDE_DIRS}
  ${IL_INCLUDE_DIR}
  ${OPENGL_INCLUDE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_BINARY_DIR}/carbon)

add_library(
  logframe MODULE 
  ${logframe_SOURCES} 
  ${logframe_QOBJECT_HEADERS}
  ${logframe_QOBJECT_HEADERS_MOC}
  ${logframe_HEADERS}
  ${logframe_FORMS_HEADERS})
  #${logframe_RESOURCES_RCC})

target_link_libraries(
  logframe
  ${OPENGL_LIBRARIES} 
  ${QT_LIBRARIES} 
  ${spark_gui_libs})

if (NOT APPLE)
   set_target_properties(logframe PROPERTIES VERSION 1.0.0 SOVERSION 1)
endif (NOT APPLE)

install(TARGETS logframe DESTINATION ${CARBON_PLUGIN_LIB_DIR})

########### install files ###############

install(DIRECTORY ./ DESTINATION ${CARBON_PLUGIN_INCLUDE_DIR}/logframe
          FILES_MATCHING PATTERN "*.h" PATTERN ".svn" EXCLUDE)
