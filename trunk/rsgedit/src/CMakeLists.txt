########### next target ###############
set(rsgedit_SRCS
   aboutDlg.h
   aboutDlg.cpp
   inputwx.h
   inputwx.cpp
   platform.h
   platform.cpp
   mainframe.h
   mainframe.cpp
   propertylist.h
   propertylist.cpp
   sparkcontextevent.h
   sparkcontextevent.cpp
   sparkglrender.h
   sparkglrender.cpp
   agentframe.h
   agentframe.cpp
   kinematicframe.h
   kinematicframe.cpp
   property.h
   property.cpp
   simspark.h
   simspark.cpp
   sparkedit.h
   sparkedit.cpp
   sparktree.h
   sparktree.cpp
   constants.h
   constants.cpp
   main.h
   main.cpp
   propertyframe.h
   propertyframe.cpp
   sparkcontext.h
   sparkcontext.cpp
   sparkglcanvas.h
   sparkglcanvas.cpp
)

add_definitions(${wxWidgets_DEFINITIONS})
include_directories(${CMAKE_SOURCE_DIR}/wxutil/include 
    ${CMAKE_SOURCE_DIR}
    ${wxWidgets_INCLUDE_DIRS} ${SPARK_INCLUDE_DIR} ${SALT_INCLUDE_DIR}
    ${ZEITGEIST_INCLUDE_DIR} ${OXYGEN_INCLUDE_DIR} ${KEROSIN_INCLUDE_DIR})

add_executable(rsgedit ${rsgedit_SRCS})

set_target_properties(rsgedit PROPERTIES COMPILE_FLAGS ${wxWidgets_CXX_FLAGS})
set_target_properties(rsgedit PROPERTIES INSTALL_RPATH "${RPATH_DIRECTORIES}")

target_link_libraries(rsgedit pthread wxutil ${SPARK_LIBRARIES} ${SALT_LIBRARIES}
  ${ZEITGEIST_LIBRARIES} ${OXYGEN_LIBRARIES} ${KEROSIN_LIBRARIES}
   ${wxWidgets_LIBRARIES})

install(TARGETS rsgedit DESTINATION ${BINDIR})


########### install files ###############

install(FILES bindings.rb rsgedit.rb DESTINATION ${DATADIR}/${CMAKE_PROJECT_NAME})
