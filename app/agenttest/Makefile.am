bin_PROGRAMS = agenttest

if DEBUG
LDADD = \
	${top_srcdir}/lib/oxygen/liboxygen_debug.la \
	${top_srcdir}/lib/zeitgeist/libzeitgeist_debug.la \
        ${top_srcdir}/lib/salt/libsalt_debug.la
agenttest_CXXFLAGS = -O -g -W -Wall
else
LDADD = \
	${top_srcdir}/lib/oxygen/liboxygen.la \
	${top_srcdir}/lib/zeitgeist/libzeitgeist.la \
        ${top_srcdir}/lib/salt/libsalt.la
agenttest_CXXFLAGS = -O2
endif

AM_CPPFLAGS = -I${top_srcdir}/plugin -I${top_srcdir}/lib  -I${top_srcdir}/utility @FREETYPE_CPPFLAGS@ @RUBY_CPPFLAGS@
AM_LDFLAGS  = @RUBY_LDFLAGS@

agenttest_SOURCES = \
	main.cpp \
	worldmodel.h worldmodel.cpp worldmodel_c.cpp \
	commserver.h commserver.cpp commserver_c.cpp \
	behavior.h behavior.cpp behavior_c.cpp \
	kicknrun.h kicknrun.cpp kicknrun_c.cpp \
	soccer.h soccer.cpp soccer_c.cpp

dist_pkgdata_DATA = agenttest.rb
