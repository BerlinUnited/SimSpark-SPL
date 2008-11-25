pkglib_LTLIBRARIES = soundsystembass.la

soundsystembass_la_SOURCES = export.cpp \
soundeffectbass.cpp \
soundeffectbass.h \
soundmodulebass.cpp \
soundmodulebass.h \
soundstreambass.cpp \
soundstreambass.h \
soundsystembass.cpp \
soundsystembass.h \
soundsystembass_c.cpp

# -module tells automake we're not building a library but a loadable module
# so we don't need the "lib" prefix in the module name
soundsystembass_la_LDFLAGS = -module -version-info 0:0:0

AM_CPPFLAGS = -I${top_srcdir}/lib @RUBY_CPPFLAGS@
