pkglib_LTLIBRARIES = sparkagent.la

sparkagent_la_SOURCES = \
	hinge2perceptor.h \
	hinge2perceptor.cpp \
	hinge2perceptor_c.cpp \
	hinge2effector.h \
	hinge2effector.cpp \
	hinge2effector_c.cpp \
	timeperceptor.h \
	timeperceptor.cpp \
	timeperceptor_c.cpp \
	export.cpp

sparkagent_la_LDFLAGS = -module -version-info 0:0:0

AM_CPPFLAGS = -I${top_srcdir}/lib -I${top_srcdir}/utility @RUBY_CPPFLAGS@

