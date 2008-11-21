if DEBUG
pkglib_LTLIBRARIES = libsalt_debug.la
libsalt_debug_la_SOURCES = $(sources)
libsalt_debug_la_CXXFLAGS = -O -g -W -Wall
libsalt_debug_la_LIBADD = @SALT_LIBADD@ ${top_builddir}/utility/libobj/liblibobject.la
libsalt_debug_la_LDFLAGS = -version-info @salt_version_info@
else
pkglib_LTLIBRARIES = libsalt.la
libsalt_la_SOURCES = $(sources)
libsalt_la_CXXFLAGS = -O2
libsalt_la_LIBADD = @SALT_LIBADD@ ${top_builddir}/utility/libobj/liblibobject.la
libsalt_la_LDFLAGS = -version-info @salt_version_info@
endif

bin_SCRIPTS = salt-config

## define include directory local to the pkgincludedir
libpkgincludedir = $(includedir)/@PACKAGE@/salt

sources = \
bounds.cpp  \
fileclasses.cpp  \
frustum.cpp  \
matrix.cpp  \
path.cpp  \
plane.cpp \
sharedlibrary.cpp

libpkginclude_HEADERS = \
bounds.h   \
fileclasses.h  \
gmath.h   \
path.h   \
random.h \
salt.h \
tvector.h \
defines.h \
frustum.h \
matrix.h \
plane.h \
rect.h \
sharedlibrary.h \
vector.h
