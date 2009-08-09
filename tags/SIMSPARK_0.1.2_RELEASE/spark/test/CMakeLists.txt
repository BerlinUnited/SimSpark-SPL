include_directories(${CMAKE_SOURCE_DIR}/lib ${CMAKE_SOURCE_DIR}/utility
		    ${Boost_INCLUDE_DIRS} ${ODE_INCLUDE_DIR})

link_directories(${Boost_LIBRARY_DIRS})

add_subdirectory(coretest)
add_subdirectory(fonttest)
add_subdirectory(inputtest)
add_subdirectory(scenetest)
add_subdirectory(zeitgeisttest)
