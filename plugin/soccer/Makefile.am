pkglib_LTLIBRARIES = soccer.la

soccer_la_SOURCES = export.cpp \
agentstate/agentstate.cpp \
agentstate/agentstate_c.cpp \
agentstateperceptor/agentstateperceptor.cpp \
agentstateperceptor/agentstateperceptor_c.cpp \
ball/ball.cpp ball/ball_c.cpp \
ballstateaspect/ballstateaspect.cpp \
ballstateaspect/ballstateaspect_c.cpp \
createeffector/createeffector.cpp \
createeffector/createeffector_c.cpp \
driveeffector/driveeffector.cpp \
driveeffector/driveeffector_c.cpp \
beameffector/beameffector.cpp \
beameffector/beameffector_c.cpp \
fieldflag/fieldflag_c.cpp \
gamestateaspect/gamestateaspect.cpp \
gamestateaspect/gamestateaspect_c.cpp \
gamestateaspect/gamestateitem.cpp \
gamestateaspect/gamestateitem_c.cpp \
gamestateperceptor/gamestateperceptor.cpp \
gamestateperceptor/gamestateperceptor_c.cpp \
initeffector/initeffector.cpp \
initeffector/initeffector_c.cpp \
kickeffector/kickeffector.cpp \
kickeffector/kickeffector_c.cpp \
sayeffector/sayeffector.cpp \
sayeffector/sayeffector_c.cpp \
objectstate/objectstate.cpp \
objectstate/objectstate_c.cpp \
trainercommandparser/trainercommandparser.cpp \
trainercommandparser/trainercommandparser_c.cpp \
sexpmonitor/sexpmonitor.cpp \
sexpmonitor/sexpmonitor_c.cpp \
soccerbase/soccerbase.cpp \
soccercontrolaspect/soccercontrolaspect.cpp \
soccercontrolaspect/soccercontrolaspect_c.cpp \
soccerruleaspect/soccerruleaspect.cpp \
soccerruleaspect/soccerruleaspect_c.cpp \
visionperceptor/visionperceptor.cpp \
visionperceptor/visionperceptor_c.cpp

nobase_libpkginclude_HEADERS = \
soccertypes.h \
agentstate/agentstate.h \
agentstateperceptor/agentstateperceptor.h \
ball/ball.h \
ballstateaspect/ballstateaspect.h \
beameffector/beamaction.h \
beameffector/beameffector.h \
createeffector/createaction.h \
createeffector/createeffector.h \
driveeffector/driveaction.h \
driveeffector/driveeffector.h \
fieldflag/fieldflag.h \
gamestateaspect/gamestateaspect.h \
gamestateaspect/gamestateitem.h \
gamestateperceptor/gamestateperceptor.h \
initeffector/initaction.h \
initeffector/initeffector.h \
kickeffector/kickaction.h \
kickeffector/kickeffector.h \
sayeffector/sayeffector.h \
sayeffector/sayaction.h \
objectstate/objectstate.h \
trainercommandparser/trainercommandparser.h \
sexpmonitor/sexpmonitor.h \
soccerbase/soccerbase.h \
soccercontrolaspect/soccercontrolaspect.h \
soccerruleaspect/soccerruleaspect.h \
visionperceptor/visionperceptor.h 

## define include directory local to the pkgincludedir
libpkgincludedir = $(includedir)/@PACKAGE@/soccer

# -module tells automake we're not building a library but a loadable module
# so we don't need the "lib" prefix in the module name
soccer_la_LDFLAGS = -module -version-info 1:0:0

AM_CPPFLAGS = -I${top_srcdir}/lib @RUBY_CPPFLAGS@ -I${top_srcdir}/plugin
