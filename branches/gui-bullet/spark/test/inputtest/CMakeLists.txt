
########### next target ###############

set(inputtest_SRCS
   main.cpp
)

include_directories(${SDL_INCLUDE_DIR} ${FREETYPE_INCLUDE_DIRS}
    ${IL_INCLUDE_DIR})

add_executable(inputtest ${inputtest_SRCS})

target_link_libraries(inputtest salt zeitgeist oxygen kerosin ${SDL_LIBRARY})


########### install files ###############




#original Makefile.am contents follow:

#if BUILD_KEROSIN
#noinst_PROGRAMS = inputtest
#endif
#
#if DEBUG
#LDADD = \
#	${top_builddir}/lib/kerosin/libkerosin_debug.la \
#	${top_builddir}/lib/oxygen/liboxygen_debug.la \
#	${top_builddir}/lib/zeitgeist/libzeitgeist_debug.la \
#        ${top_builddir}/lib/salt/libsalt_debug.la \
#	@BOOST_THREADS_LIB@
#inputtest_CXXFLAGS = -O -g -W -Wall
#else
#LDADD = \
#	${top_builddir}/lib/kerosin/libkerosin.la \
#	${top_builddir}/lib/oxygen/liboxygen.la \
#	${top_builddir}/lib/zeitgeist/libzeitgeist.la \
#        ${top_builddir}/lib/salt/libsalt.la \
#	@BOOST_THREADS_LIB@
#inputtest_CXXFLAGS = -O2
#endif
#
#AM_CPPFLAGS = -I${top_srcdir}/lib @FREETYPE_CPPFLAGS@ @RUBY_CPPFLAGS@ @SDL_CPPFLAGS@
#AM_LDFLAGS = @IL_LDFLAGS@ @SDL_LDFLAGS@ @GL_LDFLAGS@
#
#inputtest_SOURCES = main.cpp
#EXTRA_DIST = inputtest.rb init.rb german.scan.rb
