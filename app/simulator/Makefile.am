bin_PROGRAMS = rcssserver3D
bin_SCRIPTS = rcsoccersim3D
noinst_SCRIPTS = createagentdb.sh

if DEBUG
LDADD = \
	${top_builddir}/lib/oxygen/liboxygen_debug.la \
	${top_builddir}/lib/zeitgeist/libzeitgeist_debug.la \
        ${top_builddir}/lib/salt/libsalt_debug.la \
	@BOOST_THREADS_LIB@
rcssserver3D_CXXFLAGS = -O -g -W -Wall
else
LDADD = \
	${top_builddir}/lib/oxygen/liboxygen.la \
	${top_builddir}/lib/zeitgeist/libzeitgeist.la \
        ${top_builddir}/lib/salt/libsalt.la \
	@BOOST_THREADS_LIB@
rcssserver3D_CXXFLAGS = -O2
endif

AM_CPPFLAGS = -I${top_srcdir}/lib -I${top_srcdir}/utility @FREETYPE_CPPFLAGS@ @RUBY_CPPFLAGS@
AM_LDFLAGS  = @RUBY_LDFLAGS@

rcssserver3D_SOURCES = main.cpp
dist_pkgdata_DATA = rcssserver3D.rb
EXTRA_DIST = agentdb.xml

install-data-local: createagentdb.sh
	rm -f $(DESTDIR)${pkgdatadir}/agentdb.list
	${top_builddir}/${subdir}/createagentdb.sh > $(DESTDIR)${pkgdatadir}/agentdb.xml
	chmod 644 $(DESTDIR)${pkgdatadir}/agentdb.xml
