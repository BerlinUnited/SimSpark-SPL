lib_LTLIBRARIES = libsurvival.la

AM_CPPFLAGS = -I${top_srcdir}/lib @FREETYPE_CPPFLAGS@ @RUBY_CPPFLAGS@

## define include directory local to the pkgincludedir
libpkgincludedir = $(includedir)/@PACKAGE@/survival

EXTRA_DIST = survival.dsp survival.rb 

libsurvival_la_SOURCES = \
export.cpp \
linesegmentperceptor.cpp \
linesegmentperceptor_c.cpp \
survivalagentaspect.cpp \
survivalagentaspect_c.cpp \
survivalcontrolaspect.cpp \
survivalcontrolaspect_c.cpp

nobase_libpkginclude_HEADERS = \
linesegmentperceptor.h \
survivalagentaspect.h \
survivalcontrolaspect.h
