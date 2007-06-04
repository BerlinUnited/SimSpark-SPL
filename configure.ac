# Process this file with autoconf to produce a configure script.
AC_INIT([rcssserver3d], 0.5.5, [sserver-admin@lists.sf.net])
AC_CONFIG_SRCDIR([lib/salt/salt.h])
AC_CONFIG_HEADER([sparkconfig.h])
AC_CONFIG_AUX_DIR([config.aux])

AM_INIT_AUTOMAKE([gnu subdir-objects 1.7])
AC_LANG([C++])
AM_ENABLE_SHARED(yes)
AM_ENABLE_STATIC(no)

## Here are the version numbers of our libraries. They should be updated (only)
## immediately before a release of the package. See libtool manual for details.
RCSS_LIBRARY_VERSION_INFO(salt_version_info,salt_version,'3:1:3')
RCSS_LIBRARY_VERSION_INFO(zeitgeist_version_info,zeitgeist_version,'3:1:0')
RCSS_LIBRARY_VERSION_INFO(oxygen_version_info,oxygen_version,'5:2:2')
RCSS_LIBRARY_VERSION_INFO(kerosin_version_info,kerosin_version,'1:1:0')
RCSS_LIBRARY_VERSION_INFO(spark_version_info,spark_version,'0:0:0')
RCSS_LIBRARY_VERSION_INFO(rcssmonitor3D_version_info,rcssmonitor3D_version,'2:0:1')
RCSS_LIBRARY_VERSION_INFO(rcssnet3D_version_info,rcssnet3D_version,'0:0:0')

rcss_prefix=\"$prefix\"
test "$rcss_prefix" = \"NONE\" && rcss_prefix=\"$ac_default_prefix\"
AC_DEFINE_UNQUOTED(PREFIX, $rcss_prefix, [Define the install prefix. Needed by the libraries to check for ruby scripts.])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_INSTALL
AC_LIBTOOL_DLOPEN
AC_PROG_LIBTOOL
AC_PROG_AWK
AC_CHECK_PROG([DOXYGEN], [doxygen], [yes], [no])
AC_CHECK_PROG([PDFLATEX], [pdflatex], [yes], [no])
AM_CONDITIONAL(DOXYGEN, test x$DOXYGEN = xyes )
AM_CONDITIONAL(COND_PDFLATEX, test x$PDFLATEX != x)
AC_CHECK_PROG([HAVE_DOT], [dot], [YES], [NO])
AC_PATH_PROG([PERL], [perl], [perl])
# Checks for libraries.
TMP_LIBS=$LIBS
AC_CHECK_LIB(dl, dlopen, [AC_SUBST(SALT_LIBADD, [-ldl])],
    	     [AC_CHECK_FUNC(dlopen, [], [AC_MSG_ERROR([cannot find required library libdl or dlopen() function.])]
	    )])
LIBS=$TMP_LIBS

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([poll.h])
AC_CHECK_HEADERS([arpa/nameser.h])
AC_CHECK_HEADERS([resolv.h netdb.h])
AC_CHECK_HEADERS([float.h limits.h malloc.h memory.h stdlib.h string.h])

AC_CHECK_HEADER([boost/version.hpp],,
	AC_MSG_ERROR([boost library not found. Please specify the location 
of the boost header directory using the CPPFLAGS environment variable]))
AC_CHECK_HEADERS([hash_map],,
	AC_MSG_NOTICE(['hash_map' will be substituted by 'map'. If you need hash_map install stlport]))

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T

AC_CHECK_TYPES([socklen_t], [], [], [
#include <sys/types.h>
#include <sys/socket.h>
])

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

AC_SUBST(NET_LIBS)
AC_CHECK_LIB(socket, socket, [NET_LIBS="-lsocket"])
AC_CHECK_LIB(nsl, gethostbyname, [NET_LIBS="$NET_LIBS -lnsl"])
AC_CHECK_LIB(posix4, sched_yield, [NET_LIBS="$NET_LIBS -lposix4"])
AC_CHECK_LIB(resolv, hstrerror, [NET_LIBS="$NET_LIBS -lresolv"])
RCSS_BOOST_REGEX([rcss_have_boost_regex=yes],[rcss_have_boost_regex=no])
if test "$rcss_have_boost_regex" = no; then
	AC_MSG_WARN([the ZIP filesystem plugin cannot be build])
fi
AM_CONDITIONAL(HAVE_BOOST_REGEX, test $rcss_have_boost_regex = yes)
RCSS_BOOST_THREADS_LIB

## checks for system services

# check if we want to build the debug version
AC_ARG_ENABLE([debug],
     AC_HELP_STRING([--enable-debug], [Turn on debugging (default is YES)]),
     [case "${enableval}" in
       yes) debug=true && AC_DEFINE(RCSS_DEBUG, 1, [Define to 1 if we are building a debug version]) ;;
       no)  debug=false ;;
       *) AC_MSG_ERROR(bad value ${enableval} for --enable-debug) ;;
     esac],[debug=true && AC_DEFINE(RCSS_DEBUG, 1, [Define to 1 if we are building a debug version]) ])
AC_SUBST(debug, [$debug])
AM_CONDITIONAL(DEBUG, test x$debug = xtrue)

# This test is required here because of the soccer monitor.
# It can be dropped as soon as the monitor uses libkerosin.
rcss_build_kerosin=yes
RCSS_CHECK_GL
if test "$rcss_build_kerosin" = no; then
	AC_MSG_ERROR([need GL stuff to build monitor])
fi

RCSS_BUILD_KEROSIN
RCSS_BUILD_SOUNDSYSTEMFMOD
RCSS_PATH_RUBY
RCSS_CHECK_RUBY_VERSION(1,8,0)
RCSS_CHECK_ODE

AC_ARG_VAR(SPADES, [prefix of SPADES installation])
AC_ARG_WITH([spades],
	    [AC_HELP_STRING([--with-spades],
	 	 	    [whether to support SPADES-SIM (default is no)])],
	    [rcss_with_spades=$withval],
	    [rcss_with_spades=no])

if test "$rcss_with_spades" = yes; then
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
	AC_LIB_SPADES([rcss_build_spades=yes
	 	       AC_DEFINE([HAVE_SPADES_HEADERS], 1, 
			 	 [Define to 1 if you have the SPADES header files])],
                      [AC_MSG_ERROR([the SPADES library (http://spades-sim.sourceforge.net) cannot be found. Please specify the prefix used when installing SPADES with the SPADES enviroment variable.])])
else
	rcss_build_spades=no
	AC_MSG_NOTICE([SpadesServer will not be build])
fi
AM_CONDITIONAL(BUILD_SPADES_MODULES, test $rcss_build_spades = yes)




# create Makefiles and other configuration files
AC_CONFIG_FILES([Makefile \
		 utility/Makefile \
		 utility/glextgen/Makefile \
		 utility/glextgen/glextgen.rb \
		 utility/libobj/Makefile \
		 utility/sfsexp/Makefile \
		 utility/rcssnet/Makefile \
		 lib/Makefile \
		 lib/salt/Makefile \
		 lib/salt/salt-config \
		 lib/zeitgeist/Makefile \
		 lib/zeitgeist/zeitgeist-config \
		 lib/kerosin/Makefile \
		 lib/kerosin/kerosin-config \
		 lib/oxygen/Makefile \
		 lib/oxygen/oxygen-config \
		 lib/spark/Makefile \
		 plugin/Makefile \
		 plugin/filesystemstd/Makefile \
		 plugin/filesystemzip/Makefile \
		 plugin/forceeffector/Makefile \
		 plugin/sceneeffector/Makefile \
		 plugin/inputsdl/Makefile \
		 plugin/monitortest/Makefile \
		 plugin/perfectvisionperceptor/Makefile \
		 plugin/gyrorateperceptor/Makefile \
		 plugin/sexpparser/Makefile \
		 plugin/soccer/Makefile \
		 plugin/soccermonitor/Makefile \
		 plugin/soundsystemfmod/Makefile \
		 plugin/collisionperceptor/Makefile \
		 plugin/rubysceneimporter/Makefile \
		 plugin/voidmeshimporter/Makefile \
		 plugin/objimporter/Makefile \
		 plugin/sparkmonitor/Makefile \
		 plugin/sparkagent/Makefile \
		 app/Makefile \
		 app/gendot/Makefile \
		 app/agenttest/Makefile \
		 app/coretest/Makefile \
		 app/fonttest/Makefile \
		 app/inputtest/Makefile \
		 app/scenetest/Makefile \
		 app/simulator/createagentdb.sh \
		 app/simulator/rcsoccersim3D \
		 app/simulator/Makefile \
		 app/simspark/Makefile \
		 app/simspark/materials-textures.rb \
		 app/simspark/rsg/agent/soccerbottorso.rsg \
		 app/simspark/rsg/agent/ball.rsg \
		 app/monitorspark/Makefile \
		 app/agentspark/Makefile \
		 app/spadestest/Makefile \
		 app/rcssmonitor3d/Makefile \
		 app/rcssmonitor3d/lib/Makefile \
		 app/rcssmonitor3d/lite/Makefile \
		 app/rcssmonitor3d/kerosin/Makefile \
		 app/zeitgeisttest/Makefile \
                 doc/Makefile \
		 doc/Doxyfile \
                 doc/manual/Makefile \
                 doc/devel/Makefile \
                 doc/devel/howtos/Makefile \
		 doc/users/Makefile ],
	        [test -f lib/salt/salt-config && chmod a+x lib/salt/salt-config && \
		 test -f lib/zeitgeist/zeitgeist-config && chmod a+x lib/zeitgeist/zeitgeist-config && \
		 test -f lib/oxygen/oxygen-config && chmod a+x lib/oxygen/oxygen-config && \
		 test -f lib/kerosin/kerosin-config && chmod a+x lib/kerosin/kerosin-config && \
		 test -f app/simulator/rcsoccersim3D && chmod +x app/simulator/rcsoccersim3D && \
                 test -f app/simulator/createagentdb.sh && chmod +x app/simulator/createagentdb.sh])
AC_OUTPUT
