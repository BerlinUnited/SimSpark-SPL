########### next target ###############
set(oxygen_LIB_HDRS
    agentaspect/agentaspect.h
    agentaspect/effector.h
    agentaspect/jointeffector.h
    agentaspect/jointperceptor.h
    agentaspect/perceptor.h
    agentaspect/createaction.h
    controlaspect/controlaspect.h
    gamecontrolserver/predicate.h
    gamecontrolserver/gamecontrolserver.h
    gamecontrolserver/baseparser.h
    gamecontrolserver/actionobject.h
    gamecontrolserver/behavior.h
    oxygen.h
    oxygen_defines.h
    physicsserver/body.h
    physicsserver/bodycontroller.h
    physicsserver/boxcollider.h
    physicsserver/ccylindercollider.h
    physicsserver/collider.h
    physicsserver/collisionhandler.h
    physicsserver/contactjointhandler.h
    physicsserver/odeobject.h
    physicsserver/odewrapper.h
    physicsserver/physicsserver.h
    physicsserver/planecollider.h
    physicsserver/raycollider.h
    physicsserver/recorderhandler.h
    physicsserver/space.h
    physicsserver/spherecollider.h
    physicsserver/transformcollider.h
    physicsserver/world.h
    physicsserver/dragcontroller.h
    physicsserver/velocitycontroller.h
    physicsserver/joint.h
    physicsserver/balljoint.h
    physicsserver/hingejoint.h
    physicsserver/fixedjoint.h
    physicsserver/sliderjoint.h
    physicsserver/universaljoint.h
    physicsserver/hinge2joint.h
    physicsserver/angularmotor.h
    sceneserver/sceneimporter.h
    sceneserver/basenode.h
    sceneserver/fpscontroller.h
    sceneserver/scene.h
    sceneserver/sceneserver.h
    sceneserver/sceneimporter.h
    sceneserver/transform.h
    sceneserver/camera.h
    sceneserver/scenedict.h
    simulationserver/simulationserver.h
    simulationserver/simcontrolnode.h
    simulationserver/agentcontrol.h
    simulationserver/monitorcontrol.h
    simulationserver/monitorlogger.h
    simulationserver/netcontrol.h
    simulationserver/netclient.h
    simulationserver/netmessage.h
    simulationserver/netbuffer.h
    simulationserver/traincontrol.h
    geometryserver/geometryserver.h
    geometryserver/meshexporter.h
    geometryserver/meshimporter.h
    geometryserver/stdmeshimporter.h
    geometryserver/trimesh.h
    geometryserver/indexbuffer.h
    monitorserver/monitorserver.h
    monitorserver/monitorsystem.h
    monitorserver/monitoritem.h
    monitorserver/custommonitor.h
    monitorserver/monitorcmdparser.h
)
if(SPADES_FOUND)
   set(oxygen_LIB_HDRS
       ${oxygen_LIB_HDRS}
       spadesserver/spadesserver.h
       spadesserver/paramreader.h
       spadesserver/paramstorer.h
       spadesserver/spadescreatesenseevent.h
       spadesserver/spadesactevent.h
   )
endif(SPADES_FOUND)

set(oxygen_LIB_SRCS
    agentaspect/agentaspect.cpp
    agentaspect/agentaspect_c.cpp
    agentaspect/effector.cpp
    agentaspect/effector_c.cpp
    agentaspect/perceptor.cpp
    agentaspect/perceptor_c.cpp
    controlaspect/controlaspect.cpp
    controlaspect/controlaspect_c.cpp
    gamecontrolserver/actionobject_c.cpp
    gamecontrolserver/baseparser_c.cpp
    gamecontrolserver/gamecontrolserver.cpp
    gamecontrolserver/gamecontrolserver_c.cpp
    gamecontrolserver/predicate.cpp
    gamecontrolserver/behavior.cpp
    gamecontrolserver/behavior_c.cpp
    oxygen.cpp
    physicsserver/body.cpp
    physicsserver/body_c.cpp
    physicsserver/bodycontroller.cpp
    physicsserver/bodycontroller_c.cpp
    physicsserver/transformcollider.cpp
    physicsserver/transformcollider_c.cpp
    physicsserver/boxcollider.cpp
    physicsserver/boxcollider_c.cpp
    physicsserver/ccylindercollider.cpp
    physicsserver/ccylindercollider_c.cpp
    physicsserver/collider.cpp
    physicsserver/collider_c.cpp
    physicsserver/collisionhandler.cpp
    physicsserver/collisionhandler_c.cpp
    physicsserver/contactjointhandler.cpp
    physicsserver/contactjointhandler_c.cpp
    physicsserver/odeobject.cpp
    physicsserver/odeobject_c.cpp
    physicsserver/physicsserver.cpp
    physicsserver/physicsserver_c.cpp
    physicsserver/planecollider.cpp
    physicsserver/planecollider_c.cpp
    physicsserver/raycollider.cpp
    physicsserver/raycollider_c.cpp
    physicsserver/recorderhandler.cpp
    physicsserver/recorderhandler_c.cpp
    physicsserver/space.cpp
    physicsserver/space_c.cpp
    physicsserver/spherecollider.cpp
    physicsserver/spherecollider_c.cpp
    physicsserver/world.cpp
    physicsserver/world_c.cpp
    physicsserver/dragcontroller.cpp
    physicsserver/dragcontroller_c.cpp
    physicsserver/velocitycontroller.cpp
    physicsserver/velocitycontroller_c.cpp
    physicsserver/joint.cpp
    physicsserver/joint_c.cpp
    physicsserver/balljoint.cpp
    physicsserver/balljoint_c.cpp
    physicsserver/hingejoint.cpp
    physicsserver/hingejoint_c.cpp
    physicsserver/fixedjoint.cpp
    physicsserver/fixedjoint_c.cpp
    physicsserver/sliderjoint.cpp
    physicsserver/sliderjoint_c.cpp
    physicsserver/universaljoint.cpp
    physicsserver/universaljoint_c.cpp
    physicsserver/hinge2joint.cpp
    physicsserver/hinge2joint_c.cpp
    physicsserver/angularmotor.cpp
    physicsserver/angularmotor_c.cpp
    sceneserver/basenode.cpp
    sceneserver/basenode_c.cpp
    sceneserver/fpscontroller.cpp
    sceneserver/fpscontroller_c.cpp
    sceneserver/scene.cpp
    sceneserver/scene_c.cpp
    sceneserver/sceneserver.cpp
    sceneserver/sceneserver_c.cpp
    sceneserver/sceneimporter_c.cpp
    sceneserver/transform.cpp
    sceneserver/transform_c.cpp
    sceneserver/camera.cpp
    sceneserver/camera_c.cpp
    sceneserver/scenedict.cpp
    simulationserver/simulationserver.cpp
    simulationserver/simulationserver_c.cpp
    simulationserver/simcontrolnode.cpp
    simulationserver/simcontrolnode_c.cpp
    simulationserver/agentcontrol.cpp
    simulationserver/agentcontrol_c.cpp
    simulationserver/monitorcontrol.cpp
    simulationserver/monitorcontrol_c.cpp
    simulationserver/monitorlogger.cpp
    simulationserver/monitorlogger_c.cpp
    simulationserver/netcontrol.cpp
    simulationserver/netcontrol_c.cpp
    simulationserver/netclient.cpp
    simulationserver/netclient_c.cpp
    simulationserver/netmessage.cpp
    simulationserver/netmessage_c.cpp
    simulationserver/netbuffer.cpp
    simulationserver/traincontrol.cpp
    simulationserver/traincontrol_c.cpp
    geometryserver/geometryserver.h
    geometryserver/geometryserver.cpp
    geometryserver/geometryserver_c.cpp
    geometryserver/meshexporter_c.cpp
    geometryserver/meshimporter.cpp
    geometryserver/meshimporter_c.cpp
    geometryserver/stdmeshimporter.cpp
    geometryserver/stdmeshimporter_c.cpp
    geometryserver/trimesh.cpp
    geometryserver/indexbuffer.cpp
    monitorserver/monitorserver.cpp
    monitorserver/monitorserver_c.cpp
    monitorserver/monitorsystem.cpp
    monitorserver/monitorsystem_c.cpp
    monitorserver/monitoritem_c.cpp
    monitorserver/custommonitor.cpp
    monitorserver/custommonitor_c.cpp
    monitorserver/monitorcmdparser.cpp
    monitorserver/monitorcmdparser_c.cpp
)
if(SPADES_FOUND)
   set(oxygen_LIB_SRCS
       ${oxygen_LIB_SRCS}
       spadesserver/spadesserver.cpp
       spadesserver/spadesserver_c.cpp
       spadesserver/paramreader.cpp
       spadesserver/paramreader_c.cpp
       spadesserver/paramstorer.cpp
       spadesserver/spadescreatesenseevent.cpp
       spadesserver/spadesactevent.c
   )
endif(SPADES_FOUND)

add_definitions(${ODE_CFLAGS})
include_directories(${CMAKE_SOURCE_DIR}/lib ${CMAKE_SOURCE_DIR}/utility/)

add_library(oxygen ${oxygen_LIB_SRCS} ${oxygen_LIB_HDRS})

target_link_libraries(oxygen rcssnet3D ${Boost_LIBRARIES}
    ${ODE_LIBRARY} ${oxygen_require_libs})

set_target_properties(oxygen PROPERTIES VERSION ${OXYGEN_VERSION}
    SOVERSION ${OXYGEN_SO_VERSION} DEBUG_POSTFIX _debug)
install(TARGETS oxygen DESTINATION ${LIBDIR}/${CMAKE_PROJECT_NAME})

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/oxygen-config.in
    ${CMAKE_CURRENT_BINARY_DIR}/oxygen-config)

########### install files ###############

install(FILES oxygen.rb DESTINATION ${DATADIR}/${CMAKE_PROJECT_NAME})
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/oxygen-config DESTINATION ${BINDIR}
    PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
          GROUP_EXECUTE GROUP_READ WORLD_READ WORLD_EXECUTE)
install(DIRECTORY ./ DESTINATION ${INCLUDEDIR}/${CMAKE_PROJECT_NAME}/oxygen
          FILES_MATCHING PATTERN "*.h" PATTERN ".svn" EXCLUDE)

