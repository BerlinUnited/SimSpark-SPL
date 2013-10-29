########### next target ###############

set(rcssmonitor3d_SRCS
   main.cpp
)

if (APPLE)
  set(SDL_LINK_LIBS ${SDLMAIN_LIBRARY} ${SDL_LIBRARY_TEMP}) 
endif (APPLE)

include_directories(${SDL_INCLUDE_DIR})

add_executable(rcssmonitor3d ${rcssmonitor3d_SRCS})

target_link_libraries(rcssmonitor3d 
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

set_target_properties(rcssmonitor3d PROPERTIES 
    INSTALL_RPATH "${RPATH_DIRECTORIES}")
install(TARGETS rcssmonitor3d DESTINATION ${BINDIR})


########### install files ###############

install(FILES  soccersim-monitor.rb rcssmonitor3d.rb soccerbindings.rb
        DESTINATION ${DATADIR}/${CMAKE_PROJECT_NAME})
