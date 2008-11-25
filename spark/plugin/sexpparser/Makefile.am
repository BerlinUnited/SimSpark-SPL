pkglib_LTLIBRARIES = sexpparser.la

sexpparser_la_LIBADD = -lsexp

sexpparser_la_SOURCES = export.cpp \
sexpparser.cpp sexpparser_c.cpp sexpparser.h

sexpparser_la_LDFLAGS = -module -version-info 0:0:0 -L${top_builddir}/utility/sfsexp/

AM_CPPFLAGS = -I${top_srcdir}/lib -I${top_srcdir}/utility @RUBY_CPPFLAGS@
