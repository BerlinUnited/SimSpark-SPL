if BUILD_KEROSIN
bin_PROGRAMS = agentspark
endif

if DEBUG
LDADD = -lpthread  \
	${top_builddir}/lib/spark/libspark_debug.la \
	${top_builddir}/lib/kerosin/libkerosin_debug.la \
	${top_builddir}/lib/oxygen/liboxygen_debug.la \
	${top_builddir}/lib/zeitgeist/libzeitgeist_debug.la \
        ${top_builddir}/lib/salt/libsalt_debug.la \
	${top_builddir}/utility/rcssnet/librcssnet3D.la \
	@BOOST_THREADS_LIB@
agentspark_CXXFLAGS = -O -g -W -Wall
else
LDADD = -lpthread  \
	${top_builddir}/lib/spark/libspark.la \
	${top_builddir}/lib/kerosin/libkerosin.la \
	${top_builddir}/lib/oxygen/liboxygen.la \
	${top_builddir}/lib/zeitgeist/libzeitgeist.la \
        ${top_builddir}/lib/salt/libsalt.la \
	@BOOST_THREADS_LIB@
agentspark_CXXFLAGS = -O2
endif

AM_CPPFLAGS = -I${top_srcdir}/lib -I${top_srcdir}/utility \
              @FREETYPE_CPPFLAGS@ @RUBY_CPPFLAGS@ @SDL_CPPFLAGS@
AM_LDFLAGS  = @RUBY_LDFLAGS@ @SDL_LDFLAGS@ @IL_LDFLAGS@ @GL_LDFLAGS@

agentspark_SOURCES = main.cpp \
	behavior.h \
	carbehavior.cpp \
	carbehavior.h \
	soccerbehavior.h \
	soccerbehavior.cpp \
	soccerbotbehavior.h \
	soccerbotbehavior.cpp \
	leggedspherebehavior.h \
	leggedspherebehavior.cpp \
	hoap2behavior.h \
	hoap2behavior.cpp

