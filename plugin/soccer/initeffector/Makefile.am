pkglib_LTLIBRARIES = initeffector.la

initeffector_la_SOURCES = export.cpp \
initeffector.cpp initeffector.h \
initeffector_c.cpp \
initaction.h

# -module tells automake we're not building a library but a loadable module
# so we don't need the "lib" prefix in the module name
initeffector_la_LDFLAGS = -module -version-info 0:0:0

AM_CPPFLAGS = -I${top_srcdir}/lib @RUBY_CPPFLAGS@
