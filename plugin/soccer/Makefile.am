pkglib_LTLIBRARIES = soccer.la

soccer_la_SOURCES = export.cpp \
soccertypes.h \
agentstate/agentstate.cpp \
agentstate/agentstate.h \
agentstate/agentstate_c.cpp \
agentstateperceptor/agentstateperceptor.cpp \
agentstateperceptor/agentstateperceptor.h \
agentstateperceptor/agentstateperceptor_c.cpp \
ball/ball.cpp ball/ball.h ball/ball_c.cpp \
ballstateaspect/ballstateaspect.cpp \
ballstateaspect/ballstateaspect.h \
ballstateaspect/ballstateaspect_c.cpp \
createeffector/createaction.h \
createeffector/createeffector.cpp \
createeffector/createeffector.h \
createeffector/createeffector_c.cpp \
driveeffector/driveaction.h \
driveeffector/driveeffector.cpp \
driveeffector/driveeffector.h \
driveeffector/driveeffector_c.cpp \
beameffector/beamaction.h \
beameffector/beameffector.h \
beameffector/beameffector.cpp \
beameffector/beameffector_c.cpp \
fieldflag/fieldflag.h \
fieldflag/fieldflag_c.cpp \
gamestateaspect/gamestateaspect.cpp \
gamestateaspect/gamestateaspect.h \
gamestateaspect/gamestateaspect_c.cpp \
gamestateperceptor/gamestateperceptor.cpp \
gamestateperceptor/gamestateperceptor.h \
gamestateperceptor/gamestateperceptor_c.cpp \
initeffector/initaction.h \
initeffector/initeffector.cpp \
initeffector/initeffector.h \
initeffector/initeffector_c.cpp \
kickeffector/kickaction.h \
kickeffector/kickeffector.cpp \
kickeffector/kickeffector.h \
kickeffector/kickeffector_c.cpp \
objectstate/objectstate.h \
objectstate/objectstate.cpp \
objectstate/objectstate_c.cpp \
sexpmonitor/sexpmonitor.cpp \
sexpmonitor/sexpmonitor.h \
sexpmonitor/sexpmonitor_c.cpp \
soccerbase/soccerbase.cpp \
soccerbase/soccerbase.h \
soccercontrolaspect/soccercontrolaspect.cpp \
soccercontrolaspect/soccercontrolaspect.h \
soccercontrolaspect/soccercontrolaspect_c.cpp \
soccerruleaspect/soccerruleaspect.cpp \
soccerruleaspect/soccerruleaspect.h \
soccerruleaspect/soccerruleaspect_c.cpp \
visionperceptor/visionperceptor.cpp \
visionperceptor/visionperceptor.h \
visionperceptor/visionperceptor_c.cpp

# -module tells automake we're not building a library but a loadable module
# so we don't need the "lib" prefix in the module name
soccer_la_LDFLAGS = -module -version-info 0:0:0

AM_CPPFLAGS = -I${top_srcdir}/lib @RUBY_CPPFLAGS@ -I${top_srcdir}/plugin
