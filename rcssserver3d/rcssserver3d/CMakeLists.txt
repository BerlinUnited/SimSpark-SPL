########### next target ###############

set(rcssserver3d_SRCS
   main.cpp
)

add_executable(rcssserver3d ${rcssserver3d_SRCS})

if (APPLE)
  set(SDL_LINK_LIBS ${SDLMAIN_LIBRARY} ${SDL_LIBRARY_TEMP}) 
endif (APPLE)

include_directories(${SDL_INCLUDE_DIR})

target_link_libraries(rcssserver3d ${SPARK_LIBRARIES} ${SALT_LIBRARIES}
   	${ZEITGEIST_LIBRARIES} ${OXYGEN_LIBRARIES} ${KEROSIN_LIBRARIES}
    ${SDL_LINK_LIBS} ${RCSSNET3D_LIBRARY})

set_target_properties(rcssserver3d PROPERTIES INSTALL_RPATH "${RPATH_DIRECTORIES}")
install(TARGETS rcssserver3d DESTINATION ${BINDIR})

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/rcsoccersim3d.in
    ${CMAKE_CURRENT_BINARY_DIR}/rcsoccersim3d)

execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink 
    ${CMAKE_INSTALL_PREFIX}/${BINDIR}/rcssserver3d 
    ${CMAKE_CURRENT_BINARY_DIR}/simspark)

########### install files ###############

install(FILES  simspark.rb soccersim.rb naosoccersim.rb 
        internalsoccermonitor.rb internalsoccerbindings.rb
        DESTINATION ${DATADIR}/${CMAKE_PROJECT_NAME})

if (NOT WIN32)
	install(FILES  ${CMAKE_CURRENT_BINARY_DIR}/rcsoccersim3d 
          ${CMAKE_CURRENT_BINARY_DIR}/simspark DESTINATION ${BINDIR}
          PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
          GROUP_EXECUTE GROUP_READ WORLD_READ WORLD_EXECUTE)
endif (NOT WIN32)