if BUILD_KEROSIN
if DEBUG
pkglib_LTLIBRARIES = libspark_debug.la
libspark_debug_la_SOURCES = $(sources) 
libspark_debug_la_CXXFLAGS = -O -g -W -Wall
libspark_debug_la_LIBADD = @FREETYPE_LIBADD@ -lSDL -lIL -lGL 
libspark_debug_la_LDFLAGS = -version-info @spark_version_info@
else
pkglib_LTLIBRARIES = libspark.la
libspark_la_SOURCES = $(sources) 
libspark_la_CXXFLAGS = -O2
libspark_la_LIBADD = @FREETYPE_LIBADD@ -lSDL -lIL -lGL 
libspark_la_LDFLAGS = -version-info @spark_version_info@
endif

endif

AM_CPPFLAGS = -I${top_srcdir}/lib @FREETYPE_CPPFLAGS@ @RUBY_CPPFLAGS@

## define include directory local to the pkgincludedir
libpkgincludedir = $(includedir)/@PACKAGE@/spark 

## architecture independent data (scripts) to be installed and
## distributed
dist_pkgdata_DATA = spark.rb

sources = \
spark.cpp

nobase_libpkginclude_HEADERS = \
spark.h


