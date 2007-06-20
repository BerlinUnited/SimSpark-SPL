pkglib_LTLIBRARIES = rubysceneimporter.la

rubysceneimporter_la_SOURCES = export.cpp \
rubysceneimporter.cpp rubysceneimporter.h \
rubysceneimporter_c.cpp 

# -module tells automake we're not building a library but a loadable module
# so we don't need the "lib" prefix in the module name
rubysceneimporter_la_LDFLAGS = -module -avoid-version -L${top_builddir}/utility/sfsexp/
rubysceneimporter_la_LIBADD = -lsexp

AM_CPPFLAGS = -I${top_srcdir}/lib -I${top_srcdir}/utility @RUBY_CPPFLAGS@
