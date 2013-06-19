########### next target ###############
set(soccer_LIB_HDRS
   soccertypes.h
   agentstate/agentstate.h
   agentstateperceptor/agentstateperceptor.h
   ball/ball.h
   ballstateaspect/ballstateaspect.h
   beameffector/beamaction.h
   beameffector/beameffector.h
   catcheffector/catchaction.h
   catcheffector/catcheffector.h
   createeffector/createaction.h
   createeffector/createeffector.h
   driveeffector/driveaction.h
   driveeffector/driveeffector.h
   fieldflag/fieldflag.h
   gamestateaspect/gamestateaspect.h
   gamestateaspect/gamestateitem.h
   gamestateperceptor/gamestateperceptor.h
   hearperceptor/hearperceptor.h
   initeffector/initaction.h
   initeffector/initeffector.h
   initeffector/singlematiniteffector.h
   initeffector/staticmeshiniteffector.h
   kickeffector/kickaction.h
   kickeffector/kickeffector.h
   objectstate/objectstate.h
   pantilteffector/pantiltaction.h
   pantilteffector/pantilteffector.h
   restrictedvisionperceptor/restrictedvisionperceptor.h
   sayeffector/sayaction.h
   sayeffector/sayeffector.h
   sexpmonitor/sexpmonitor.h
   internalsoccermonitor/internalsoccerrender.h
   internalsoccermonitor/internalsoccerinput.h
   soccerbase/soccerbase.h
   soccercontrolaspect/soccercontrolaspect.h
   soccernode/soccernode.h
   soccerruleaspect/soccerruleaspect.h
   soccerruleaspect/soccerruleitem.h
   trainercommandparser/trainercommandparser.h
   gametimeperceptor/gametimeperceptor.h
   visionperceptor/visionperceptor.h
   agentintegration/soccerbotbehavior.h
   hmdp_effector/hmdpaction.h
   hmdp_effector/hmdpeffector.h
   hmdp_effector/hmdpperceptor.h
   hmdp_effector/naospecific.h
   line/line.h
   agentcollisionhandler/agentcollisionhandler.h
   rcs3dmonitor/rcs3dmonitor.h
)

set(soccer_LIB_SRCS
   export.cpp
   agentstate/agentstate.cpp
   agentstate/agentstate_c.cpp
   agentstateperceptor/agentstateperceptor.cpp
   agentstateperceptor/agentstateperceptor_c.cpp
   ball/ball.cpp
   ball/ball_c.cpp
   ballstateaspect/ballstateaspect.cpp
   ballstateaspect/ballstateaspect_c.cpp
   beameffector/beameffector.cpp
   beameffector/beameffector_c.cpp
   catcheffector/catcheffector.cpp
   catcheffector/catcheffector_c.cpp
   createeffector/createeffector.cpp
   createeffector/createeffector_c.cpp
   driveeffector/driveeffector.cpp
   driveeffector/driveeffector_c.cpp
   fieldflag/fieldflag_c.cpp
   gamestateaspect/gamestateaspect.cpp
   gamestateaspect/gamestateaspect_c.cpp
   gamestateaspect/gamestateitem.cpp
   gamestateaspect/gamestateitem_c.cpp
   gamestateperceptor/gamestateperceptor.cpp
   gamestateperceptor/gamestateperceptor_c.cpp
   hearperceptor/hearperceptor.cpp
   hearperceptor/hearperceptor_c.cpp
   initeffector/initeffector.cpp
   initeffector/initeffector_c.cpp
   initeffector/singlematiniteffector.cpp
   initeffector/singlematiniteffector_c.cpp
   initeffector/staticmeshiniteffector.cpp
   initeffector/staticmeshiniteffector_c.cpp
   kickeffector/kickeffector.cpp
   kickeffector/kickeffector_c.cpp
   objectstate/objectstate.cpp
   objectstate/objectstate_c.cpp
   pantilteffector/pantilteffector.cpp
   pantilteffector/pantilteffector_c.cpp
   restrictedvisionperceptor/restrictedvisionperceptor.cpp
   restrictedvisionperceptor/restrictedvisionperceptor_c.cpp
   sayeffector/sayeffector.cpp
   sayeffector/sayeffector_c.cpp
   sexpmonitor/sexpmonitor.cpp
   sexpmonitor/sexpmonitor_c.cpp
   internalsoccermonitor/internalsoccerrender.cpp
   internalsoccermonitor/internalsoccerrender_c.cpp
   internalsoccermonitor/internalsoccerinput.cpp
   internalsoccermonitor/internalsoccerinput_c.cpp
   soccerbase/soccerbase.cpp
   soccercontrolaspect/soccercontrolaspect.cpp
   soccercontrolaspect/soccercontrolaspect_c.cpp
   soccernode/soccernode.cpp
   soccernode/soccernode_c.cpp
   soccerruleaspect/soccerruleaspect.cpp
   soccerruleaspect/soccerruleaspect_c.cpp
   soccerruleaspect/soccerruleitem.cpp
   soccerruleaspect/soccerruleitem_c.cpp
   trainercommandparser/trainercommandparser.cpp
   trainercommandparser/trainercommandparser_c.cpp
   visionperceptor/visionperceptor.cpp
   visionperceptor/visionperceptor_c.cpp
   gametimeperceptor/gametimeperceptor.cpp
   gametimeperceptor/gametimeperceptor_c.cpp
   agentintegration/soccerbotbehavior.cpp
   agentintegration/soccerbotbehavior_c.cpp
   hmdp_effector/hmdpeffector_c.cpp
   hmdp_effector/hmdpeffector.cpp
   hmdp_effector/hmdpperceptor_c.cpp
   hmdp_effector/hmdpperceptor.cpp
   hmdp_effector/hmdpwrapper.cpp
   hmdp_effector/naospecific.cpp
   line/line.cpp
   line/line_c.cpp
   agentcollisionhandler/agentcollisionhandler.cpp
   agentcollisionhandler/agentcollisionhandler_c.cpp
   rcs3dmonitor/rcs3dmonitor.cpp
   rcs3dmonitor/rcs3dmonitor_c.cpp
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${FREETYPE_INCLUDE_DIRS}
  ${IL_INCLUDE_DIR})

add_library(soccer MODULE ${soccer_LIB_SRCS} ${soccer_LIB_HDRS})

target_link_libraries(soccer ${spark_libs} ${OPENGL_gl_LIBRARY}
  ${Boost_LIBRARIES})

if (NOT APPLE)
   set_target_properties(soccer PROPERTIES VERSION 1.1.0 SOVERSION 1)
endif (NOT APPLE)

install(TARGETS soccer DESTINATION ${LIBDIR}/${CMAKE_PROJECT_NAME})


########### install files ###############

install(DIRECTORY ./ DESTINATION ${INCLUDEDIR}/${CMAKE_PROJECT_NAME}/soccer
          FILES_MATCHING PATTERN "*.h" PATTERN ".svn" EXCLUDE)
