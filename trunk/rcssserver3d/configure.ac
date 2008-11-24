# Process this file with autoconf to produce a configure script.
AC_INIT([rcssserver3d], 0.6, [simspark-devel@lists.sourceforge.net])
AC_CONFIG_SRCDIR([simspark/main.cpp])
AC_CONFIG_HEADER([rcssserver3d_config.h])
AC_CONFIG_AUX_DIR([config.aux])

AM_INIT_AUTOMAKE([gnu subdir-objects 1.7])
AC_LANG([C++])
AM_ENABLE_SHARED(yes)
AM_ENABLE_STATIC(no)

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_INSTALL
AC_LIBTOOL_DLOPEN
AC_PROG_LIBTOOL
AC_CHECK_PROG([DOXYGEN], [doxygen], [yes], [no])
AC_CHECK_PROG([PDFLATEX], [pdflatex], [yes])
AM_CONDITIONAL(DOXYGEN, test x$DOXYGEN = xyes )
AM_CONDITIONAL(COND_PDFLATEX, test x$PDFLATEX != x)

# Checks for simspark libraries.
SPARK_LIB_SALT
SPARK_LIB_ZEITGEIST
SPARK_LIB_OXYGEN
SPARK_LIB_KEROSIN
SPARK_LIB_SPARK

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([float.h limits.h malloc.h memory.h stdlib.h string.h])
AC_CHECK_HEADERS([execinfo.h])
AC_CHECK_HEADERS([CoreFoundation/CoreFoundation.h])

AC_CHECK_HEADER([boost/version.hpp],,
	AC_MSG_ERROR([boost library not found. Please specify the location
of the boost header directory using the CPPFLAGS environment variable]))
AC_CHECK_HEADERS([backward/hash_map hash_map],break,
	AC_MSG_NOTICE(['hash_map' will be substituted by 'map'. If you need hash_map install stlport]))

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T

# Checks for library functions.
RCSS_BOOST_THREADS_LIB

## checks for system services

# check if we want to build the debug version
AC_ARG_ENABLE([debug],
     AS_HELP_STRING([--enable-debug], [Turn on debugging (default is YES)]),
     [case "${enableval}" in
       yes) debug=true && AC_DEFINE(RCSS_DEBUG, 1, [Define to 1 if we are building a debug version]) ;;
       no)  debug=false ;;
       *) AC_MSG_ERROR(bad value ${enableval} for --enable-debug) ;;
     esac],[debug=true && AC_DEFINE(RCSS_DEBUG, 1, [Define to 1 if we are building a debug version]) ])
AC_SUBST(debug, [$debug])
AM_CONDITIONAL(DEBUG, test x$debug = xtrue)

# check if we want to build without hardcoding library path using rpath
AC_ARG_ENABLE([rpath],
     AS_HELP_STRING([--disable-rpath], [disable using --rpath to hardcode library path in the executables]),
     [],[enable_rpath=yes])
AM_CONDITIONAL(NO_RPATH, test x$enable_rpath = xno)

RCSS_CHECK_GL
RCSS_PATH_FREETYPE
RCSS_CHECK_SDL
RCSS_PATH_RUBY
RCSS_CHECK_RUBY_VERSION(1,8,0)

# create Makefiles and other configuration files
AC_CONFIG_FILES([Makefile \
		 rcssagent3d/Makefile \
		 rcssmonitor3d/Makefile \
		 simspark/Makefile \
		 plugin/Makefile \
		 plugin/soccer/Makefile \
		 plugin/soccermonitor/Makefile \
                 data/Makefile \
		 doc/Makefile \
		 doc/users/Makefile \
		 doc/Doxyfile ])
AC_OUTPUT
