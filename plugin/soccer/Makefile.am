pkglib_LTLIBRARIES = soccer.la

soccer_la_SOURCES = export.cpp \
createeffector/createeffector.cpp \
createeffector/createeffector_c.cpp \
initeffector/initeffector.cpp \
initeffector/initeffector_c.cpp \
dasheffector/dasheffector.cpp \
dasheffector/dasheffector_c.cpp \
kickeffector/kickeffector.cpp \
kickeffector/kickeffector_c.cpp \
soccercontrolaspect/soccercontrolaspect.cpp \
soccercontrolaspect/soccercontrolaspect_c.cpp \
gamestateaspect/gamestateaspect.cpp \
gamestateaspect/gamestateaspect_c.cpp \
soccerruleaspect/soccerruleaspect.cpp \
soccerruleaspect/soccerruleaspect_c.cpp \
agentstate/agentstate.cpp \
agentstate/agentstate_c.cpp \
ballstateaspect/ballstateaspect.cpp \
ballstateaspect/ballstateaspect_c.cpp \
sexpmonitor/sexpmonitor.cpp \
sexpmonitor/sexpmonitor_c.cpp \
fieldflag/fieldflag_c.cpp \
ball/ball.cpp ball/ball_c.cpp ball/ball.h \
visionperceptor/visionperceptor.h \
visionperceptor/visionperceptor.cpp \
visionperceptor/visionperceptor_c.cpp


# -module tells automake we're not building a library but a loadable module
# so we don't need the "lib" prefix in the module name
soccer_la_LDFLAGS = -module -version-info 0:0:0

AM_CPPFLAGS = -I${top_srcdir}/lib @RUBY_CPPFLAGS@ -I${top_srcdir}/plugin
