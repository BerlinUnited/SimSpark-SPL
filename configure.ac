# Process this file with autoconf to produce a configure script.
AC_INIT([rcssserver3d],[0.1],[fruit@robolog.org])
AC_CONFIG_SRCDIR([plugin/inputsdl/keyboardsdl_c.cpp])
AC_CONFIG_HEADER([config.h])
AC_CONFIG_AUX_DIR([config.aux])
AM_INIT_AUTOMAKE([gnu subdir-objects])
AC_LANG([C++])
AM_ENABLE_SHARED(no)
AM_ENABLE_STATIC(yes)

AC_ARG_ENABLE(debug,
     [  --enable-debug    Turn on debugging],
     [case "${enableval}" in
       yes) debug=true ;;
       no)  debug=false ;;
       *) AC_MSG_ERROR(bad value ${enableval} for --enable-debug) ;;
     esac],[debug=false])
AM_CONDITIONAL(DEBUG, test x$debug = xtrue)

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_LIBTOOL
AC_PROG_AWK

# Checks for libraries.

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([float.h limits.h malloc.h memory.h stdlib.h string.h])
AC_CHECK_HEADER([boost/version.hpp],,AC_MSG_ERROR([boost library not found]))
AC_CHECK_HEADERS([hash_map],,
	AC_MSG_WARN([consider installing the stlport library. 'hash_map' will be substituted by a 'map' for now.]))

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_ALLOCA
AC_FUNC_MEMCMP
AC_FUNC_SETVBUF_REVERSED
AC_FUNC_VPRINTF
AC_CHECK_FUNCS([atexit floor gethostname inet_ntoa memset pow socket sqrt strchr strrchr strstr])

AC_CONFIG_FILES([Makefile \
		 lib/Makefile \
		 lib/salt/Makefile \
		 lib/zeitgeist/Makefile \
		 lib/kerosin/Makefile])
AC_OUTPUT
