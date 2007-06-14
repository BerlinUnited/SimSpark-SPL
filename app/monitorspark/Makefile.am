if BUILD_KEROSIN
bin_PROGRAMS = monitorspark
endif

if DEBUG
LDADD = -lpthread  \
	${top_srcdir}/lib/spark/libspark_debug.la \
	${top_srcdir}/lib/kerosin/libkerosin_debug.la \
	${top_srcdir}/lib/oxygen/liboxygen_debug.la \
	${top_srcdir}/lib/zeitgeist/libzeitgeist_debug.la \
        ${top_srcdir}/lib/salt/libsalt_debug.la \
	${top_srcdir}/utility/rcssnet/librcssnet3D.la \
	-l@BOOST_THREADS_LIB@
monitorspark_CXXFLAGS = -O -g -W -Wall
else
LDADD = -lpthread  \
	${top_srcdir}/lib/spark/libspark.la \
	${top_srcdir}/lib/kerosin/libkerosin.la \
	${top_srcdir}/lib/oxygen/liboxygen.la \
	${top_srcdir}/lib/zeitgeist/libzeitgeist.la \
        ${top_srcdir}/lib/salt/libsalt.la \
	-l@BOOST_THREADS_LIB@
monitorspark_CXXFLAGS = -O2
endif

AM_CPPFLAGS = -I${top_srcdir}/lib -I${top_srcdir}/utility \
              @FREETYPE_CPPFLAGS@ @RUBY_CPPFLAGS@ @SDL_CPPFLAGS@
AM_LDFLAGS  = @RUBY_LDFLAGS@ @SDL_LDFLAGS@ @IL_LDFLAGS@ @GL_LDFLAGS@

monitorspark_SOURCES = \
	main.cpp

dist_pkgdata_DATA = \
	bindings.rb \
	soccersim.rb \
	monitorspark.rb \
	soccerbindings.rb \
	VeraMono.ttf