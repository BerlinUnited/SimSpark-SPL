########### target files ###############

set(sparkgui_SOURCES
   main.cpp
)

########### qt properties ###############

#Additional used libraries
set(QT_USE_QTOPENGL TRUE)
set(QT_USE_QTXML TRUE)

#Include qt properties
include(${QT_USE_FILE})	
add_definitions(${QT_DEFINITIONS})

########### definitions for config file ###############

set(GUI_LAYOUT_DIR ${carbonlayoutlibdir})
set(GUI_PLUGIN_DIR_1 ${carbonpluginlibdir})
set(GUI_PLUGIN_DIR_2 ${guipluginlibdir})
set(GUI_SETUP_DIR ${carbonsetupdir})
set(EXECUTABLE_INCLUDE_DIR ${CMAKE_INSTALL_PREFIX}/${BINDIR})
set(SCRIPT_INCLUDE_DIR_1 ${sparkscriptdir})
set(SCRIPT_INCLUDE_DIR_2 ${CMAKE_INSTALL_PREFIX}/${DATADIR}/${CMAKE_PROJECT_NAME})

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/sparkgui_config.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/sparkgui_config.h)

########### create executable ###############

add_executable(sparkgui ${sparkgui_SOURCES})

if (APPLE)
  set(SDL_LINK_LIBS ${SDLMAIN_LIBRARY} ${SDL_LIBRARY_TEMP}) 
endif (APPLE)

include_directories(${SDL_INCLUDE_DIR} )

set(sparkgui_LIBS ${OPENGL_LIBRARIES} ${QT_LIBRARIES} ${SDL_LINK_LIBS} ${RCSSNET3D_LIBRARY})
set(sparkgui_DEBUG_LIBS ${SPARK_LIBRARY_DEBUG} ${SALT_LIBRARY_DEBUG}
   	${ZEITGEIST_LIBRARY_DEBUG} ${OXYGEN_LIBRARY_DEBUG} ${KEROSIN_LIBRARY_DEBUG} 
	${CARBON_LIBRARY_DEBUG})
set(sparkgui_RELEASE_LIBS ${SPARK_LIBRARY_RELEASE} ${SALT_LIBRARY_RELEASE}
   	${ZEITGEIST_LIBRARY_RELEASE} ${OXYGEN_LIBRARY_RELEASE} ${KEROSIN_LIBRARY_RELEASE} 
	${CARBON_LIBRARY_RELEASE})

target_link_libraries(sparkgui ${sparkgui_LIBS})
FOREACH(LIB ${sparkgui_DEBUG_LIBS})
  target_link_libraries(sparkgui debug ${LIB})
ENDFOREACH(LIB)
FOREACH(LIB ${sparkgui_RELEASE_LIBS})
  target_link_libraries(sparkgui optimized ${LIB})
ENDFOREACH(LIB) 

target_link_libraries(sparkgui ${Boost_LIBRARIES})

set_target_properties(sparkgui PROPERTIES INSTALL_RPATH "${RPATH_DIRECTORIES}")
install(TARGETS sparkgui DESTINATION ${BINDIR})

execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink 
    sparkgui ${CMAKE_CURRENT_BINARY_DIR}/simspark)

########### install files ###############

#install script files
install(DIRECTORY ./
		DESTINATION ${DATADIR}/${CMAKE_PROJECT_NAME}
        FILES_MATCHING PATTERN "*.rb" PATTERN ".svn" EXCLUDE)

#if (NOT WIN32)
#    install(FILES  ${CMAKE_CURRENT_BINARY_DIR}/rcsoccersim3d 
#              ${CMAKE_CURRENT_BINARY_DIR}/simspark DESTINATION ${BINDIR}
#              PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
#              GROUP_EXECUTE GROUP_READ WORLD_READ WORLD_EXECUTE)
#endif (NOT WIN32)
