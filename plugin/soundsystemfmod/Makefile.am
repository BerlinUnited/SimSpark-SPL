# Building the soundsystem plugin is optional and dependent on an
# existing fmod installation

if BUILD_KEROSIN
if BUILD_SOUNDSYSTEMFMOD
pkglib_LTLIBRARIES = soundsystemfmod.la
endif
endif

soundsystemfmod_la_LIBADD = -lfmod

soundsystemfmod_la_SOURCES = export.cpp \
soundeffectfmod.cpp \
soundeffectfmod.h \
soundmodulefmod.cpp \
soundmodulefmod.h \
soundstreamfmod.cpp \
soundstreamfmod.h \
soundsystemfmod.cpp \
soundsystemfmod.h \
soundsystemfmod_c.cpp

# -module tells automake we're not building a library but a loadable module
# so we don't need the "lib" prefix in the module name
soundsystemfmod_la_LDFLAGS = -module -version-info 0:0:0

AM_CPPFLAGS = -I${top_srcdir}/lib @RUBY_CPPFLAGS@

