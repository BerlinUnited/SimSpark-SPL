pkglib_LTLIBRARIES = survival.la

survival_la_SOURCES = \
export.cpp \
linesegmentperceptor.cpp \
linesegmentperceptor.h \
linesegmentperceptor_c.cpp \
survivalagentaspect.cpp \
survivalagentaspect.h \
survivalagentaspect_c.cpp \
survivalcontrolaspect.cpp \
survivalcontrolaspect.h \
survivalcontrolaspect_c.cpp

EXTRA_DIST = survival.dsp survival.rb 

# -module tells automake we're not building a library but a loadable module
# so we don't need the "lib" prefix in the module name
survival_la_LDFLAGS = -module -version-info 0:0:0

AM_CPPFLAGS = -I${top_srcdir}/lib @FREETYPE_CPPFLAGS@ @RUBY_CPPFLAGS@

## define include directory local to the pkgincludedir
# libpkgincludedir = $(includedir)/@PACKAGE@/survival
