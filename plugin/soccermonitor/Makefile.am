pkglib_LTLIBRARIES = soccermonitor.la

soccermonitor_la_SOURCES = export.cpp \
	soccermonitor.cpp \
	soccermonitor_c.cpp \
	soccerrender.cpp \
	soccerrender_c.cpp

nobase_libpkginclude_HEADERS = \
	soccermonitor.h \
	soccerrender.h

## define include directory local to the pkgincludedir
libpkgincludedir = $(includedir)/@PACKAGE@/soccermonitor

# -module tells automake we're not building a library but a loadable module
# so we don't need the "lib" prefix in the module name
soccermonitor_la_LDFLAGS = -module -version-info 1:0:0

AM_CPPFLAGS = -I${top_srcdir}/lib @RUBY_CPPFLAGS@ -I${top_srcdir}/plugin @FREETYPE_CPPFLAGS@
