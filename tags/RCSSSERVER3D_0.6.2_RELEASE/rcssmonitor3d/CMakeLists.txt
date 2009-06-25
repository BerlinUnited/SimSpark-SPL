########### next target ###############

set(rcssmonitor3d_SRCS
   main.cpp
)

if (APPLE)
  set(SDL_LINK_LIBS ${SDLMAIN_LIBRARY} ${SDL_LIBRARY_TEMP}) 
endif (APPLE)

include_directories(${SPARK_INCLUDE_DIR} ${SALT_INCLUDE_DIR}
  ${ZEITGEIST_INCLUDE_DIR} ${OXYGEN_INCLUDE_DIR} ${KEROSIN_INCLUDE_DIR}
  ${SDL_INCLUDE_DIR})

add_executable(rcssmonitor3d ${rcssmonitor3d_SRCS})

target_link_libraries(rcssmonitor3d ${SPARK_LIBRARIES} ${SALT_LIBRARIES}
  ${ZEITGEIST_LIBRARIES} ${OXYGEN_LIBRARIES} ${KEROSIN_LIBRARIES}
  ${SDL_LINK_LIBS} ${RCSSNET3D_LIBRARY})

set_target_properties(rcssmonitor3d PROPERTIES 
    INSTALL_RPATH "${RPATH_DIRECTORIES}")
install(TARGETS rcssmonitor3d DESTINATION ${BINDIR})


########### install files ###############

install(FILES  soccersim-monitor.rb rcssmonitor3d.rb soccerbindings.rb
        DESTINATION ${DATADIR}/${CMAKE_PROJECT_NAME})
