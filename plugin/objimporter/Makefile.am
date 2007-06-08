pkglib_LTLIBRARIES = objimporter.la

objimporter_la_SOURCES = export.cpp \
objimporter.cpp objimporter_c.cpp objimporter.h

objimporter_la_LDFLAGS = -module -avoid-version

AM_CPPFLAGS = -I${top_srcdir}/lib -I${top_srcdir}/utility @RUBY_CPPFLAGS@

