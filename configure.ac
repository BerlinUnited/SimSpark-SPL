# Process this file with autoconf to produce a configure script.
AC_INIT([rcssserver3d], 0.2.1, [sserver-admin@lists.sf.net])
AC_CONFIG_SRCDIR([lib/salt/salt.h])
AC_CONFIG_HEADER([config.h])
AC_CONFIG_AUX_DIR([config.aux])

AM_INIT_AUTOMAKE([gnu subdir-objects 1.7])
AC_LANG([C++])
AM_ENABLE_SHARED(yes)
AM_ENABLE_STATIC(no)

rcss_prefix=\"$prefix\"
test "$rcss_prefix" = \"NONE\" && rcss_prefix=\"$ac_default_prefix\"
AC_DEFINE_UNQUOTED(PREFIX, $rcss_prefix, [Define the install prefix. Needed by the libraries to check for ruby scripts.])

AC_ARG_ENABLE(debug,
     AC_HELP_STRING([--enable-debug], [Turn on debugging (default is YES)]),
     [case "${enableval}" in
       yes) debug=true && 
	    AC_DEFINE(RCSS_DEBUG, 1, 
                      [Define to 1 if we are building a debug version]) ;;
       no)  debug=false ;;
       *) AC_MSG_ERROR(bad value ${enableval} for --enable-debug) ;;
     esac],[debug=true])
AM_CONDITIONAL(DEBUG, test x$debug = xtrue)

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_INSTALL
AC_LIBTOOL_DLOPEN
AC_PROG_LIBTOOL
AC_PROG_AWK
AC_CHECK_PROG([DOXYGEN], [doxygen], [yes], [no])
AM_CONDITIONAL(DOXYGEN, test x$DOXYGEN = xyes)

# Checks for libraries.

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([float.h limits.h malloc.h memory.h stdlib.h string.h])


AC_CHECK_HEADER([boost/version.hpp],,
  AC_MSG_ERROR([boost library not found. Please specify the location 
of the boost header directory using the CPPFLAGS environment variable]))
AC_CHECK_HEADERS([hash_map],,
	AC_MSG_WARN(['hash_map' will be substituted by 'map'. Possible fix: install stlport]))

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_ALLOCA
# the FUNC_MEMCMP macro has a bug when used with c++
# (see http://mail.gnu.org/archive/html/autoconf-patches/2003-05/msg00042.html)
# the workaround is to switch back to C for the test
AC_LANG([C])
AC_FUNC_MEMCMP
AC_LANG([C++])

AC_FUNC_SETVBUF_REVERSED
AC_FUNC_VPRINTF
AC_CONFIG_LIBOBJ_DIR(./utility/libobj/)
AC_REPLACE_FUNCS([_vsnprintf])
AC_CHECK_FUNCS([atexit floor gethostname inet_ntoa memset pow socket sqrt strchr strrchr strstr strupr])

## checks for system services
RCSS_CHECK_RCSSBASE
RCSS_BUILD_KEROSIN
RCSS_BUILD_SOUNDSYSTEMFMOD
RCSS_PATH_RUBY
RCSS_CHECK_ODE

AC_ARG_VAR(SPADES, [prefix of SPADES installation])
if test "$SPADES"; then
  if test -z "$PATH"; then
    PATH="$SPADES/bin"
  else
    PATH="$PATH:$SPADES/bin"
  fi
  if test -z "$LDFLAGS"; then
    LDFLAGS="-L$SPADES/lib"
  else
    LDFLAGS="$LDFLAGS -L$SPADES/lib"
  fi
  if test -z "$CPPFLAGS"; then
    CPPFLAGS="-I$SPADES/include"
  else
    CPPFLAGS="$CPPFLAGS -I$SPADES/include"
  fi
fi

AC_LIB_SPADES(
	[rcss_build_spades=yes
	 AC_DEFINE([HAVE_SPADES_HEADERS], 1, [Define to 1 if you have the SPADES header files])],
	[rcss_build_spades=no
         AC_MSG_WARN([the SPADES (http://spades-sim.sourceforge.net) library cannot be found. Please specify the prefix used when installing SPADES with the SPADES enviroment variable.])
	 AC_MSG_NOTICE([SpadesServer will not be build])]
	)
AM_CONDITIONAL(BUILD_SPADES_MODULES, test $rcss_build_spades = yes)

# create Makefiles and other configuration files
AC_CONFIG_FILES([Makefile \
		 utility/Makefile \
		 utility/glextgen/Makefile \
		 utility/glextgen/glextgen.rb \
		 utility/libobj/Makefile \
		 utility/sfsexp/Makefile \
		 lib/Makefile \
		 lib/salt/Makefile \
		 lib/zeitgeist/Makefile \
		 lib/kerosin/Makefile \
		 lib/oxygen/Makefile \
		 plugin/Makefile \
		 plugin/filesystemstd/Makefile \
		 plugin/filesystemzip/Makefile \
		 plugin/forceeffector/Makefile \
		 plugin/inputsdl/Makefile \
		 plugin/monitortest/Makefile \
		 plugin/perfectvisionperceptor/Makefile \
		 plugin/sexpparser/Makefile \
		 plugin/soccer/Makefile \
		 plugin/soundsystemfmod/Makefile \
		 plugin/collisionperceptor/Makefile \
		 app/Makefile \
		 app/agenttest/Makefile \
		 app/coretest/Makefile \
		 app/fonttest/Makefile \
		 app/inputtest/Makefile \
		 app/scenetest/Makefile \
		 app/simulator/Makefile \
                 app/simulator/createagentdb.sh \
                 app/simulator/rcsoccersim3D \
		 app/spadestest/Makefile \
		 app/rcssmonitor3d/Makefile \
		 app/zeitgeisttest/Makefile \
                 doc/Makefile \
		 doc/Doxyfile],
	        [test -f app/simulator/rcsoccersim3D && chmod +x app/simulator/rcsoccersim3D \
                 test -f app/simulator/createagentdb.sh && chmod +x app/simulator/createagentdb.sh])
AC_OUTPUT
