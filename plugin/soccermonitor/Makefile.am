if BUILD_KEROSIN
pkglib_LTLIBRARIES = soccermonitor.la
endif

soccermonitor_la_SOURCES = export.cpp \
	soccermonitor.cpp \
	soccermonitor_c.cpp \
	soccerrender.cpp \
	soccerrender_c.cpp \
	soccerinput.cpp \
	soccerinput_c.cpp

nobase_libpkginclude_HEADERS = \
	soccermonitor.h \
	soccerrender.h \
	soccerinput.h

## define include directory local to the pkgincludedir
libpkgincludedir = $(includedir)/@PACKAGE@/soccermonitor

# -module tells automake we're not building a library but a loadable module
# so we don't need the "lib" prefix in the module name
soccermonitor_la_LDFLAGS = -module -version-info 1:0:0

AM_CPPFLAGS = -I${top_srcdir}/lib -I${top_srcdir}/plugin \
	-I${top_srcdir}/utility @FREETYPE_CPPFLAGS@ @RUBY_CPPFLAGS@
