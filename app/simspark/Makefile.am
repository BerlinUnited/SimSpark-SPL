if BUILD_KEROSIN
bin_PROGRAMS = simspark
endif

if DEBUG
LDADD = -lpthread -lGL -lGLU -lglut \
	${top_srcdir}/lib/spark/libspark_debug.la \
	${top_srcdir}/lib/kerosin/libkerosin_debug.la \
	${top_srcdir}/lib/oxygen/liboxygen_debug.la \
	${top_srcdir}/lib/zeitgeist/libzeitgeist_debug.la \
        ${top_srcdir}/lib/salt/libsalt_debug.la
simspark_CXXFLAGS = -O -g -W -Wall
else
LDADD = -lpthread -lGL -lGLU -lglut \
	${top_srcdir}/lib/spark/libspark.la \
	${top_srcdir}/lib/kerosin/libkerosin.la \
	${top_srcdir}/lib/oxygen/liboxygen.la \
	${top_srcdir}/lib/zeitgeist/libzeitgeist.la \
        ${top_srcdir}/lib/salt/libsalt.la
simspark_CXXFLAGS = -O2
endif

AM_CPPFLAGS = -I${top_srcdir}/lib -I${top_srcdir}/utility \
              @FREETYPE_CPPFLAGS@ @RUBY_CPPFLAGS@

AM_LDFLAGS  = @RUBY_LDFLAGS@

simspark_SOURCES = \
	main.cpp
