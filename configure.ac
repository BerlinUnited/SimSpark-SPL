# Process this file with autoconf to produce a configure script.
AC_INIT([rcssserver3d], 0.1, [sserver-admin@lists.sf.net])
AC_CONFIG_SRCDIR([plugin/inputsdl/keyboardsdl_c.cpp])
AC_CONFIG_HEADER([config.h])
AC_CONFIG_AUX_DIR([config.aux])

AM_INIT_AUTOMAKE([gnu subdir-objects])
AC_LANG([C++])
AM_ENABLE_SHARED(yes)
AM_ENABLE_STATIC(no)

AC_DEFINE(PREFIX, /usr/local/share, [Define the install prefix])

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

# Checks for libraries.
AC_CHECK_LIB(slang, SLang_init_all,,break)

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([float.h limits.h malloc.h memory.h stdlib.h string.h])

# check for OpenGL location and used extensions
AC_CHECK_HEADER([GL/gl.h],,
  AC_MSG_ERROR([required OpenGL headers not found. Please specify the location 
of the OpenGL header directory using the CPPFLAGS environment variable]))
AC_CHECK_HEADERS([GL/glx.h],
                 AC_SUBST([GLTARGET], [x]),
		 AC_CHECK_HEADERS([GL/wglext.h],
                                  AC_SUBST([GLTARGET], [win32]),
  				  AC_MSG_ERROR([could not find X nor MS Windows GL extensions]),    
				  [#include <GL/gl.h>]),
		 [#include <GL/gl.h>])

GLDIR=`echo "#include <GL/gl.h>" | /lib/cpp -M | awk '{ print $2 }'`
GLDIR=`dirname "$GLDIR"`
AC_SUBST([GLDIR], [$GLDIR])
AC_CHECK_FILE([$GLDIR/glx.h])

AC_CHECK_LIB(gllib,glEnable)

AC_CHECK_HEADER([boost/version.hpp],,
  AC_MSG_ERROR([boost library not found. Please specify the location 
of the boost header directory using the CPPFLAGS environment variable]))
AC_CHECK_HEADER([SDL/SDL.h],,
  AC_MSG_ERROR([Simple DirectMedia Layer library not found. Please specify 
                the location of the SDL header directory using the CPPFLAGS 
                environment variable]))
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
AC_PATH_X
RCSS_PATH_FREETYPE
RCSS_PATH_RUBY
RCSS_CHECK_SOUNDSYSTEMFMOD

AC_ARG_VAR(ODE, [location of ode installation])
if test $ODE; then
   CPPFLAGS="$CPPFLAGS -I$ODE/include"
   LDFLAGS="$LDFLAGS -L$ODE/lib"
fi

AC_CHECK_HEADERS([ode/ode.h],,[
        AC_MSG_ERROR([The ode headers (e.g. ode/ode.h) cannot be found. Please specify the location of the ode installtion, by using the ODE environment variable (e.g. ./configure ODE=$HOME/ode-0.03)])])

AC_CHECK_LIB(ode,dWorldCreate,,[
        AC_MSG_ERROR([The ode library (libode.a) cannot be found. Please specify the location of the ode installtion, by using the ODE environment variable (e.g. ./configure ODE=$HOME/ode-0.03)])])

AC_ARG_VAR(SDL, [location of SDL installation])
if test $SDL; then
   CPPFLAGS="$CPPFLAGS -I$SDL/include"
   LDFLAGS="$LDFLAGS -L$SDL/lib"
fi

AC_CHECK_HEADERS([SDL/SDL.h],,[
        AC_MSG_ERROR([The SDL headers (e.g. SDL/SDL.h) cannot be found. Please specify the location of the SDL installtion, by using the SDL environment variable (e.g. ./configure SDL=$HOME/SDL)])])

AC_CHECK_LIB(SDL, SDL_Init,,[
        AC_MSG_ERROR([The SDL library (libSDL.a) cannot be found. Please specify the location of the SDL installtion, by using the SDL environment variable (e.g. ./configure SDL=$HOME/SSL)])])


AC_ARG_VAR(DEVIL, [location of DevIL installation])
if test $DEVIL; then
   CPPFLAGS="$CPPFLAGS -I$DEVIL/include"
   LDFLAGS="$LDFLAGS -L$DEVIL/lib"
fi

AC_CHECK_HEADERS([IL/il.h],,[
        AC_MSG_ERROR([The DevIL headers (e.g. IL/il.h) cannot be found. Please specify the location of the DevIL installtion, by using the DEVIL environment variable (e.g. ./configure DEVIL=$HOME/DevIL)])])

#AC_CHECK_LIB(IL,ilInit,,[
#        AC_MSG_ERROR([The DevIL library (libIL.la) cannot be found. Please specify the location of the DevIL installtion, by using the DEVIL environment variable (e.g. ./configure DEVIL=$HOME/DevIL)])])

# create Makefiles and other configuration files
AC_CONFIG_FILES([Makefile \
		 utility/Makefile \
		 utility/glextgen/Makefile \
		 utility/glextgen/glextgen.rb \
		 utility/libobj/Makefile \
		 lib/Makefile \
		 lib/salt/Makefile \
		 lib/zeitgeist/Makefile \
		 lib/kerosin/Makefile \
		 lib/oxygen/Makefile \
		 plugin/Makefile \
		 plugin/filesystemrar/Makefile \
		 plugin/filesystemstd/Makefile \
		 plugin/filesystemzip/Makefile \
		 plugin/inputsdl/Makefile \
		 plugin/soundsystembass/Makefile \
		 plugin/soundsystemfmod/Makefile \
		 plugin/survival/Makefile \
		 app/Makefile \
		 app/coretest/Makefile \
		 app/inputtest/Makefile \
		 app/scenetest/Makefile \
		 app/simulator/Makefile \
		 app/zeitgeisttest/Makefile \
		 app/spadestest/Makefile \
                 doc/Makefile \
		 doc/Doxyfile])
AC_OUTPUT
