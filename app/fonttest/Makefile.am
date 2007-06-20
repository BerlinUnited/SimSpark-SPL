if BUILD_KEROSIN
noinst_PROGRAMS = fonttest
all-local: data/font/VeraMono.ttf
endif

if DEBUG
LDADD = \
	${top_srcdir}/lib/kerosin/libkerosin_debug.la \
	${top_srcdir}/lib/oxygen/liboxygen_debug.la \
	${top_srcdir}/lib/zeitgeist/libzeitgeist_debug.la \
        ${top_srcdir}/lib/salt/libsalt_debug.la @FREETYPE_LIBADD@ @SDL_LDFLAGS@ @IL_LIBADD@ @GL_LIBADD@ \
	@BOOST_THREADS_LIB@

fonttest_CXXFLAGS = -O -g -W -Wall
else
LDADD = \
	${top_srcdir}/lib/kerosin/libkerosin.la \
	${top_srcdir}/lib/oxygen/liboxygen.la \
	${top_srcdir}/lib/zeitgeist/libzeitgeist.la \
        ${top_srcdir}/lib/salt/libsalt.la @FREETYPE_LIBADD@ @SDL_LDFLAGS@ @IL_LIBADD@ @GL_LIBADD@ \
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
data/font/VeraMono.ttf.gz \
data/image/jpg.jpg \
script/default.rb \
script/fonttest.rb \
script/german.scan.rb \
script/init.rb \
script/us.scan.rb

data/font/VeraMono.ttf: data/font/VeraMono.ttf.gz
	gunzip -c data/font/VeraMono.ttf.gz > data/font/VeraMono.ttf
