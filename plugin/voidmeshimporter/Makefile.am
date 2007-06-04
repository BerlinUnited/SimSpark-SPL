pkglib_LTLIBRARIES = voidmeshimporter.la

voidmeshimporter_la_SOURCES = export.cpp \
voidmeshimporter.cpp voidmeshimporter_c.cpp voidmeshimporter.h

voidmeshimporter_la_LDFLAGS = -module -avoid-version -L${top_srcdir}/utility/sfsexp/

AM_CPPFLAGS = -I${top_srcdir}/lib -I${top_srcdir}/utility @RUBY_CPPFLAGS@

