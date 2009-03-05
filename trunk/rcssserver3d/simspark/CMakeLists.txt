########### next target ###############

set(simspark_SRCS
   main.cpp
)

add_executable(simspark ${simspark_SRCS})

if (APPLE)
   include_directories( ${SPARK_INCLUDE_DIR} ${SALT_INCLUDE_DIR}
   			${ZEITGEIST_INCLUDE_DIR} ${OXYGEN_INCLUDE_DIR} ${KEROSIN_INCLUDE_DIR}
			${SDL_INCLUDE_DIR})

   target_link_libraries(simspark ${SPARK_LIBRARIES} ${SALT_LIBRARIES}
   				  ${ZEITGEIST_LIBRARIES} ${OXYGEN_LIBRARIES} ${KEROSIN_LIBRARIES}
				  ${SDLMAIN_LIBRARY} ${SDL_LIBRARY_TEMP})
else (APPLE)
   include_directories( ${SPARK_INCLUDE_DIR} ${SALT_INCLUDE_DIR}
   			${ZEITGEIST_INCLUDE_DIR} ${OXYGEN_INCLUDE_DIR} ${KEROSIN_INCLUDE_DIR})

   target_link_libraries(simspark ${SPARK_LIBRARIES} ${SALT_LIBRARIES}
   				  ${ZEITGEIST_LIBRARIES} ${OXYGEN_LIBRARIES} ${KEROSIN_LIBRARIES})
endif (APPLE)



set_target_properties(simspark PROPERTIES INSTALL_RPATH "${RPATH_DIRECTORIES}")
install(TARGETS simspark DESTINATION ${BINDIR})


########### install files ###############

install(FILES  simspark.rb soccersim.rb naosoccersim.rb 
        internalsoccermonitor.rb internalsoccerbindings.rb
        DESTINATION ${DATADIR}/${CMAKE_PROJECT_NAME})
