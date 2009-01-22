########### next target ###############

set(gendot_SRCS
   main.cpp
)

include_directories(${SPARK_INCLUDE_DIR} ${SALT_INCLUDE_DIR}
  ${ZEITGEIST_INCLUDE_DIR} ${OXYGEN_INCLUDE_DIR} ${KEROSIN_INCLUDE_DIR})

add_executable(gendot ${gendot_SRCS})

target_link_libraries(gendot ${SPARK_LIBRARIES} ${SALT_LIBRARIES}
  ${ZEITGEIST_LIBRARIES} ${OXYGEN_LIBRARIES} ${KEROSIN_LIBRARIES})
set_target_properties(gendot PROPERTIES INSTALL_RPATH "${RPATH_DIRECTORIES}")

add_custom_target(zeitgeist.ps dot -Tps zeitgeist.dot -o zeitgeist.ps)
add_custom_target(zeitgeist.gif dot -Tgif zeitgeist.dot -o zeitgeist.gif)
add_custom_target(zeitgeist.dot ./gendot)
