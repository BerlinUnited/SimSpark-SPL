pkglib_LTLIBRARIES = soccer.la

soccer_la_SOURCES = export.cpp \
createeffector/createeffector.cpp \
createeffector/createeffector_c.cpp \
initeffector/initeffector.cpp \
initeffector/initeffector_c.cpp \
gamestateaspect/gamestateaspect.cpp \
gamestateaspect/gamestateaspect_c.cpp \
agentstate/agentstate.cpp \
agentstate/agentstate_c.cpp

# -module tells automake we're not building a library but a loadable module
# so we don't need the "lib" prefix in the module name
soccer_la_LDFLAGS = -module -version-info 0:0:0

AM_CPPFLAGS = -I${top_srcdir}/lib @RUBY_CPPFLAGS@ -I${top_srcdir}/plugin
