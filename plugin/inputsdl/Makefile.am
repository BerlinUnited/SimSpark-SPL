pkglib_LTLIBRARIES = inputsdl.la

inputsdl_la_SOURCES = \
export.cpp \
inputdevicesdl.h \
inputsystemsdl.cpp \
inputsystemsdl.h \
inputsystemsdl_c.cpp \
keyboardsdl.cpp \
keyboardsdl.h \
keyboardsdl_c.cpp \
mousesdl.cpp \
mousesdl.h \
mousesdl_c.cpp \
timersdl.cpp \
timersdl.h \
timersdl_c.cpp

# -module tells automake we're not building a library but a loadable module
# so we don't need the "lib" prefix in the module name
inputsdl_la_LDFLAGS = -module -version-info 0:0:0

AM_CPPFLAGS = -I${top_srcdir}/lib @RUBY_CPPFLAGS@

