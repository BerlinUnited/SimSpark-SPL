if DEBUG
pkglib_LTLIBRARIES = libsalt_debug.la
libsalt_debug_la_SOURCES = $(sources)
libsalt_debug_la_CXXFLAGS = -O -g -W -Wall
libsalt_debug_la_LIBADD = ${top_srcdir}/utility/libobj/liblibobject.la
else
pkglib_LTLIBRARIES = libsalt.la
libsalt_la_SOURCES = $(sources)
libsalt_la_CXXFLAGS = -O2
libsalt_la_LIBADD = ${top_srcdir}/utility/libobj/liblibobject.la
endif

## define include directory local to the pkgincludedir
libpkgincludedir = $(includedir)/@PACKAGE@/salt

sources = \
bounds.cpp  \
fileclasses.cpp  \
frustum.cpp  \
matrix.cpp  \
path.cpp  \
plane.cpp \
gcc/sharedlibrary.cpp

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
