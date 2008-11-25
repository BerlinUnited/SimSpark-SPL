if BUILD_KEROSIN
noinst_PROGRAMS = scenetest
endif

if DEBUG
LDADD = \
	${top_builddir}/lib/kerosin/libkerosin_debug.la \
	${top_builddir}/lib/oxygen/liboxygen_debug.la \
	${top_builddir}/lib/zeitgeist/libzeitgeist_debug.la \
        ${top_builddir}/lib/salt/libsalt_debug.la \
	@BOOST_THREADS_LIB@
scenetest_CXXFLAGS = -O -g -W -Wall
else
LDADD = \
	${top_builddir}/lib/kerosin/libkerosin.la \
	${top_builddir}/lib/oxygen/liboxygen.la \
	${top_builddir}/lib/zeitgeist/libzeitgeist.la \
        ${top_builddir}/lib/salt/libsalt.la \
	@BOOST_THREADS_LIB@
scenetest_CXXFLAGS = -O2
endif

AM_CPPFLAGS = -I${top_srcdir}/lib -I${top_srcdir}/utility \
              @FREETYPE_CPPFLAGS@ @RUBY_CPPFLAGS@ @SDL_CPPFLAGS@
AM_LDFLAGS  = @RUBY_LDFLAGS@ @SDL_LDFLAGS@ @IL_LDFLAGS@ @GL_LDFLAGS@

scenetest_SOURCES = main.cpp \
md5mesh.cpp md5mesh.h md5mesh_c.cpp 

if BUNDLE_SUPPORT
all-local:
	${top_srcdir}/macosX/mkapp scenetest
	${top_srcdir}/macosX/fwcopy libkerosin ${top_srcdir}/lib/kerosin/.libs scenetest
	${top_srcdir}/macosX/fwcopy liboxygen ${top_srcdir}/lib/oxygen/.libs scenetest
	${top_srcdir}/macosX/fwcopy libzeitgeist ${top_srcdir}/lib/zeitgeist/.libs scenetest
	${top_srcdir}/macosX/fwcopy libsalt ${top_srcdir}/lib/salt/.libs scenetest
	${top_srcdir}/macosX/fwcopy librcssnet3D ${top_srcdir}/utility/rcssnet/.libs scenetest liboxygen
	${top_srcdir}/macosX/plugincopy ${top_srcdir}/plugin/filesystemstd/.libs/filesystemstd.so scenetest
	${top_srcdir}/macosX/plugincopy ${top_srcdir}/plugin/sexpparser/.libs/sexpparser.so scenetest
	${top_srcdir}/macosX/plugincopy ${top_srcdir}/plugin/inputsdl/.libs/inputsdl.so scenetest
endif
