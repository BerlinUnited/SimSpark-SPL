########### next target ###############

set(rcssserver3d_SRCS
   main.cpp
)

add_executable(rcssserver3d ${rcssserver3d_SRCS})

if (APPLE)
  set(SDL_LINK_LIBS ${SDLMAIN_LIBRARY} ${SDL_LIBRARY_TEMP})
endif (APPLE)

include_directories(${SDL_INCLUDE_DIR})

target_link_libraries(rcssserver3d
    ${SDL_LINK_LIBS} ${RCSSNET3D_LIBRARY} ${Boost_LIBRARIES}
    debug ${SPARK_LIBRARY_DEBUG}
    debug ${SALT_LIBRARY_DEBUG}
    debug ${ZEITGEIST_LIBRARY_DEBUG}
    debug ${OXYGEN_LIBRARY_DEBUG}
    debug ${KEROSIN_LIBRARY_DEBUG}
    optimized ${SPARK_LIBRARY_RELEASE}
    optimized ${SALT_LIBRARY_RELEASE}
    optimized ${ZEITGEIST_LIBRARY_RELEASE}
    optimized ${OXYGEN_LIBRARY_RELEASE}
    optimized ${KEROSIN_LIBRARY_RELEASE}
)

set_target_properties(rcssserver3d PROPERTIES INSTALL_RPATH "${RPATH_DIRECTORIES}")
install(TARGETS rcssserver3d DESTINATION ${BINDIR})

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/rcsoccersim3d.in
    ${CMAKE_CURRENT_BINARY_DIR}/rcsoccersim3d)

execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink
    rcssserver3d ${CMAKE_CURRENT_BINARY_DIR}/simspark)

########### install files ###############

install(FILES  rcssserver3d.rb soccersim.rb naosoccersim.rb naorobottypes.rb
        internalsoccermonitor.rb internalsoccerbindings.rb
        DESTINATION ${DATADIR}/${CMAKE_PROJECT_NAME})

if (NOT WIN32)
    install(FILES  ${CMAKE_CURRENT_BINARY_DIR}/rcsoccersim3d
              ${CMAKE_CURRENT_BINARY_DIR}/simspark DESTINATION ${BINDIR}
              PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
              GROUP_EXECUTE GROUP_READ WORLD_READ WORLD_EXECUTE)
endif (NOT WIN32)
