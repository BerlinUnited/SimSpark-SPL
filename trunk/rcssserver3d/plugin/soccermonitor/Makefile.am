pkglib_LTLIBRARIES = soccermonitor.la

soccermonitor_la_SOURCES = export.cpp \
	soccermonitor.cpp \
	soccermonitor_c.cpp \
	soccerrender.cpp \
	soccerrender_c.cpp \
	soccerinput.cpp \
	soccerinput_c.cpp \
    soccerinputlogplayer.cpp \
    soccerinputlogplayer_c.cpp

nobase_libpkginclude_HEADERS = \
	soccermonitor.h \
	soccerrender.h \
	soccerinput.h \
    soccerinputlogplayer.h

## define include directory local to the pkgincludedir
libpkgincludedir = $(includedir)/@PACKAGE@/soccermonitor

# -module tells automake we're not building a library but a loadable module
# so we don't need the "lib" prefix in the module name
soccermonitor_la_LDFLAGS = -module -version-info 1:0:0

AM_CPPFLAGS = -I${top_srcdir}/plugin @SALT_CPPFLAGS@ @SPARK_CPPFLAGS@ \
		  @ZEITGEIST_CPPFLAGS@ @OXYGEN_CPPFLAGS@ @KEROSIN_CPPFLAGS@ \
		  @FREETYPE_CPPFLAGS@
