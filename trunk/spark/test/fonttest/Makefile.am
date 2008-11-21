if BUILD_KEROSIN
noinst_PROGRAMS = fonttest
endif

if DEBUG
LDADD = \
	${top_builddir}/lib/kerosin/libkerosin_debug.la \
	${top_builddir}/lib/oxygen/liboxygen_debug.la \
	${top_builddir}/lib/zeitgeist/libzeitgeist_debug.la \
        ${top_builddir}/lib/salt/libsalt_debug.la @FREETYPE_LIBADD@ @SDL_LDFLAGS@ @IL_LIBADD@ @GL_LIBADD@ \
	@BOOST_THREADS_LIB@

fonttest_CXXFLAGS = -O -g -W -Wall
else
LDADD = \
	${top_builddir}/lib/kerosin/libkerosin.la \
	${top_builddir}/lib/oxygen/liboxygen.la \
	${top_builddir}/lib/zeitgeist/libzeitgeist.la \
        ${top_builddir}/lib/salt/libsalt.la @FREETYPE_LIBADD@ @SDL_LDFLAGS@ @IL_LIBADD@ @GL_LIBADD@ \
	@BOOST_THREADS_LIB@

fonttest_CXXFLAGS = -O2
endif 

AM_CPPFLAGS = -I${top_srcdir}/lib @FREETYPE_CPPFLAGS@ @RUBY_CPPFLAGS@ @SDL_CPPFLAGS@

if BUNDLE_SUPPORT
AM_LDFLAGS = @FREETYPE_LIBADD@ @SDL_LIBADD@ @IL_LIBADD@ @GL_LIBADD@ -framework OpenGL
else
AM_LDFLAGS = @FREETYPE_LIBADD@ @SDL_LIBADD@ @IL_LIBADD@ @GL_LIBADD@ 
endif

fonttest_SOURCES = main.cpp 

EXTRA_DIST = \
data/image/jpg.jpg \
script/default.rb \
script/fonttest.rb \
script/init.rb 
