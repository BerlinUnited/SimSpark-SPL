pkglib_LTLIBRARIES = monitortest.la

monitortest_la_SOURCES = export.cpp \
monitortest.cpp monitortest.h \
monitortest_c.cpp 

# -module tells automake we're not building a library but a loadable module
# so we don't need the "lib" prefix in the module name
monitortest_la_LDFLAGS = -module -version-info 0:0:0

AM_CPPFLAGS = -I${top_srcdir}/lib @RUBY_CPPFLAGS@
