noinst_PROGRAMS = simulator

if DEBUG
LDADD = \
	${top_srcdir}/lib/oxygen/liboxygen_debug.la \
	${top_srcdir}/lib/zeitgeist/libzeitgeist_debug.la \
        ${top_srcdir}/lib/salt/libsalt_debug.la 
simulator_CXXFLAGS = -O -g -W -Wall
else
LDADD = \
	${top_srcdir}/lib/oxygen/liboxygen.la \
	${top_srcdir}/lib/zeitgeist/libzeitgeist.la \
        ${top_srcdir}/lib/salt/libsalt.la 
simulator_CXXFLAGS = -O2
endif

AM_CPPFLAGS = -I${top_srcdir}/lib @FREETYPE_CPPFLAGS@ @RUBY_CPPFLAGS@
AM_LDFLAGS  = @RUBY_LDFLAGS@

simulator_SOURCES = main.cpp
EXTRA_DIST = simulator.rb agentdb.list
