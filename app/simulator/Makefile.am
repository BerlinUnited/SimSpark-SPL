bin_PROGRAMS = rcssserver3D
bin_SCRIPTS = rcsoccersim3D
noinst_SCRIPTS = createagentdb.sh

if DEBUG
LDADD = \
	${top_srcdir}/lib/oxygen/liboxygen_debug.la \
	${top_srcdir}/lib/zeitgeist/libzeitgeist_debug.la \
        ${top_srcdir}/lib/salt/libsalt_debug.la
rcssserver3D_CXXFLAGS = -O -g -W -Wall
else
LDADD = \
	${top_srcdir}/lib/oxygen/liboxygen.la \
	${top_srcdir}/lib/zeitgeist/libzeitgeist.la \
        ${top_srcdir}/lib/salt/libsalt.la 
rcssserver3D_CXXFLAGS = -O2
endif

AM_CPPFLAGS = -I${top_srcdir}/lib -I${top_srcdir}/utility @FREETYPE_CPPFLAGS@ @RUBY_CPPFLAGS@
AM_LDFLAGS  = @RUBY_LDFLAGS@

rcssserver3D_SOURCES = main.cpp
dist_pkgdata_DATA = rcssserver3D.rb
EXTRA_DIST = agentdb.xml

install-data-local: createagentdb.sh
	rm -f ${pkgdatadir}/agentdb.list
	${srcdir}/createagentdb.sh > ${pkgdatadir}/agentdb.xml
	chmod 644 ${pkgdatadir}/agentdb.xml
