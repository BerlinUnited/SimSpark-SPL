pkglib_LTLIBRARIES = collisionperceptor.la

collisionperceptor_la_SOURCES = export.cpp \
				collisionperceptor.h \
				collisionperceptor.cpp \
				collisionperceptor_c.cpp \
				perceptorhandler.h \
				perceptorhandler.cpp \
				perceptorhandler_c.cpp \
				touchperceptor.h \
				touchperceptor.cpp \
				touchperceptor_c.cpp \
				touchperceptorhandler.h \
				touchperceptorhandler.cpp \
				touchperceptorhandler_c.cpp \
				forceresistanceperceptor.h \
				forceresistanceperceptor.cpp \
				forceresistanceperceptor_c.cpp

# -module tells automake we're not building a library but a loadable module
# so we don't need the "lib" prefix in the module name
collisionperceptor_la_LDFLAGS = -module -version-info 0:0:0

AM_CPPFLAGS = -I${top_srcdir}/lib @RUBY_CPPFLAGS@

