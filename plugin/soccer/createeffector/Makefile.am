pkglib_LTLIBRARIES = createeffector.la

createeffector_la_SOURCES = export.cpp \
createeffector.cpp createeffector.h \
createeffector_c.cpp \
createaction.h

# -module tells automake we're not building a library but a loadable module
# so we don't need the "lib" prefix in the module name
createeffector_la_LDFLAGS = -module -version-info 0:0:0

AM_CPPFLAGS = -I${top_srcdir}/lib @RUBY_CPPFLAGS@
