# RCSS_PATH_RUBY
# Substitutes: @RUBY@		ruby command
#              @RUBY_CPPFLAGS@  additional CPPFLAGS to use to get ruby.h
#	       @RUBY_LDFLAGS@   additional LDFLAGS to use when linking libruby
#	       @RUBY_LDADD@     additional libraries libruby depends on  
#
# original version from the Swig list (major changes by me /oliver)
# at http://mailman.cs.uchicago.edu/mailman/listinfo/swig
#-----------------------------------------------------------------------------
AC_DEFUN(RCSS_PATH_RUBY, [

	# --with-ruby=PATH
	AC_ARG_WITH(ruby,
		AC_HELP_STRING([--with-ruby=PATH],       
	 		       [path to Ruby interpreter (default is ruby)]),
		    [ruby="$withval"],
		    [ruby="ruby"]
	)
        AC_PATH_PROGS(RUBY,[$ruby ruby],no)
	# Test ruby interpreter	
	if test $ruby = no; then
		AC_MSG_ERROR(Could not find Ruby Interpreter.  Please use --with-ruby option.)
	fi
		
	# check ruby headers
	AC_CHECK_HEADER(ruby.h, 
			[RUBY_CPPFLAGS=''],
			[unset ac_cv_header_ruby_h
		         ruby_tmp=$CPPFLAGS
                         ruby_includedir=`$RUBY -rrbconfig -e "print Config::CONFIG[['archdir']]"`
                         AC_MSG_NOTICE(trying again in $ruby_includedir)
		         RUBY_CPPFLAGS="-I$ruby_includedir"
                         CPPFLAGS="$CPPFLAGS $RUBY_CPPFLAGS"
                         AC_CHECK_HEADER(ruby.h, [],
                                         [AC_MSG_ERROR(check your ruby installation)]
                         )
                         CPPFLAGS=$ruby_tmp
                        ]
        )

        # set ruby ldflags
	AC_MSG_CHECKING(ruby library directory)
	ruby_libdir=`$RUBY -rrbconfig -e "print Config::CONFIG[['archdir']]"`
	AC_MSG_RESULT($ruby_libdir)
	RUBY_LDFLAGS="-L$ruby_libdir -lruby"

	# set ruby additional libraries
	AC_MSG_CHECKING(libraries ruby depends on)
	RUBY_LDADD=`$RUBY -rrbconfig -e "print Config::CONFIG[['LIBS']]"`
	AC_MSG_RESULT($RUBY_LDADD)
	
	# Substitute Makefile Vars.
	AC_SUBST(RUBY)
	AC_SUBST(RUBY_CPPFLAGS)
	AC_SUBST(RUBY_LDFLAGS)
	AC_SUBST(RUBY_LDADD)
]) # RCSS_PATH_RUBY

# RCSS_CHECK_ODE
#-----------------------------------------------------------------------------
AC_DEFUN(RCSS_CHECK_ODE, [
	AC_ARG_VAR(ODE, [location of ode installation])
	if test $ODE; then
	   CPPFLAGS="$CPPFLAGS -I$ODE/include"
	   LDFLAGS="$LDFLAGS -L$ODE/lib"
	fi

	AC_CHECK_HEADERS([ode/ode.h],,[
        		 AC_MSG_ERROR([The ode headers (e.g. ode/ode.h) cannot be found. Please specify the location of the ode installation, by using the ODE environment variable (e.g. ./configure ODE=$HOME/ode-0.03)])])

	AC_MSG_CHECKING([for the ODE library])
	rcss_tmp="$LDFLAGS"
	LDFLAGS="$LDFLAGS -lode"
 	AC_LINK_IFELSE([int main() { return 0; }],
		       [AC_MSG_RESULT([yes])],
		       [AC_MSG_RESULT([no])
		        AC_MSG_ERROR([The ode library (libode.a or libode.so) cannot be found. Please specify the location of the ode installation using the ODE environment variable (e.g. ./configure ODE=$HOME/ode-0.03)])])
	LDFLAGS="$rcss_tmp"
]) # RCSS_CHECK_ODE

# RCSS_PATH_FREETYPE
# Substitutes: @FREETYPE_CPPFLAGS@ 
# 	       @FREETYPE_LIBADD@
#
#-----------------------------------------------------------------------------
AC_DEFUN(RCSS_PATH_FREETYPE, [
	# --with-freetype=PATH
	AC_ARG_WITH(freetype,
		AC_HELP_STRING([--with-freetype=PATH],       
	 		       [path to freetype-config (default is freetype-config)]),
		    [freetype="$withval"],
		    [freetype="freetype-config"]
	)
	RCSS_KEROSIN_IF_ELSE([
	        AC_PATH_PROGS(FREETYPE,[$freetype freetype-config],no)
		if test $FREETYPE = "no"; then
			RCSS_BUILD_KEROSIN_ERROR([need freetype2 devel library to compile libkerosin])
		else
			rcss_freetype_cppflags=`$FREETYPE --cflags`
			rcss_freetype_libadd=`$FREETYPE --libtool`
			AC_MSG_CHECKING([freetype include flags])
			AC_MSG_RESULT([$rcss_freetype_cppflags])
			AC_SUBST(FREETYPE_CPPFLAGS, [$rcss_freetype_cppflags])
			AC_MSG_CHECKING([freetype libtool file])
			AC_MSG_RESULT([$rcss_freetype_libadd])
			AC_SUBST(FREETYPE_LIBADD, [$rcss_freetype_libadd])
		fi
	])
]) # RCSS_PATH_FREETYPE

# RCSS_CHECK_GL
# 	if rcss_build_kerosin is (still) set to 'yes', check for the GL
#	headers and libraries.
#	Substitutes: @GLDIR@ with the directory where the gl headers can be found
#-----------------------------------------------------------------------------
AC_DEFUN(RCSS_CHECK_GL, [
	RCSS_KEROSIN_IF_ELSE([
	# check for OpenGL location and used extensions
    		AC_CHECK_HEADER([GL/gl.h],,
                                RCSS_BUILD_KEROSIN_ERROR([the OpenGL headers could not be found. Please specify the location  of the OpenGL header directory using the CPPFLAGS environment variable]))
		RCSS_KEROSIN_IF_ELSE([
			AC_CHECK_HEADERS([GL/glx.h], AC_SUBST([GLTARGET], [x]),
                        		 AC_CHECK_HEADERS([GL/wglext.h],
                                   	 AC_SUBST([GLTARGET], [win32]),
                                         RCSS_BUILD_KEROSIN_ERROR([could not find 
 X nor MS Windows GL extensions]),
 				          [#include <GL/gl.h>]),
                          [#include <GL/gl.h>])

	# subst'ing the directory where the prepocessor finds gl.h
        GLDIR=`echo "#include <GL/gl.h>" | /lib/cpp -M | awk '{ print @S|@2 }'`
        GLDIR=`dirname "$GLDIR"`
        AC_SUBST([GLDIR], [$GLDIR])

	# checking if linking against libGL succeeds 
	RCSS_KEROSIN_IF_ELSE([
		AC_MSG_CHECKING([if linking against libGL succeeds])
		rcss_tmp="$LDFLAGS"
		LDFLAGS="$LDFLAGS -lGL"
 		AC_LINK_IFELSE([#include <GL/gl.h>
				int main() { glColor3f(0,0,0); }],
				[AC_MSG_RESULT([yes])],
				[AC_MSG_RESULT([no])
				 RCSS_BUILD_KEROSIN_ERROR([to build libkerosin, set LDFLAGS so that libGL can be found])])
		LDFLAGS="$rcss_tmp"
		])
    ])
  ])
]) # RCSS_CHECK_GL

# RCSS_CHECK_DEVIL
# 	if rcss_build_kerosin is (still) set to 'yes', check for the DEVIL
#	headers and libraries.
#	If DEVIL headers or libraries can not be found, building kerosin will 
#	be disabled.
#-----------------------------------------------------------------------------
AC_DEFUN(RCSS_CHECK_DEVIL, [
	AC_ARG_VAR(DEVIL, [location of DevIL installation])
	if test $DEVIL; then
		CPPFLAGS="$CPPFLAGS -I$DEVIL/include"
		LDFLAGS="$LDFLAGS -L$DEVIL/lib"
	fi
	RCSS_KEROSIN_IF_ELSE([
                              AC_CHECK_HEADER([IL/il.h],,
                              RCSS_BUILD_KEROSIN_ERROR([DevIL headers not found. 
Please set CPPFLAGS appropriately or you can specify the location of the DevIL installation using the DEVIL environment variable (e.g. ./configure DEVIL=$HOME/DevIL)]))
                              ])
	RCSS_KEROSIN_IF_ELSE([
                              rcss_tmp="$LDFLAGS"
                              LDFLAGS="$LDFLAGS -lIL"
                              AC_LINK_IFELSE([#include <IL/il.h>
#include <stdarg.h> /* _vsnprintf may be undefined (and it is needed by libIL) */
extern "C" int _vsnprintf(char *str, size_t size, const char *format, va_list ap) { return 0;}
                                              int main() { ilInit(); return 0; }],,
        		      RCSS_BUILD_KEROSIN_ERROR([The DevIL library (libIL.a or libIL.so) cannot be found. 
Please set LDFLAGS appropriately or you can specify the location of the DevIL installation using the DEVIL environment variable (e.g. ./configure DEVIL=$HOME/DevIL)]))
                              LDFLAGS="$rcss_tmp"
                              ])
]) # RCSS_CHECK_DEVIL

# RCSS_CHECK_SDL
# 	if rcss_build_kerosin is (still) set to 'yes', check for the SDL
#	headers and libraries.
#	If SDL headers or libraries can not be found, building kerosin will be
#       disabled.
#-----------------------------------------------------------------------------
AC_DEFUN(RCSS_CHECK_SDL, [
	AC_ARG_VAR(SDL, [location of SDL installation])
	if test $SDL; then
		CPPFLAGS="$CPPFLAGS -I$SDL/include"
		LDFLAGS="$LDFLAGS -L$SDL/lib"
	fi
	RCSS_KEROSIN_IF_ELSE([
                              AC_CHECK_HEADER([SDL/SDL.h],,
                              RCSS_BUILD_KEROSIN_ERROR([Simple DirectMedia Layer (SDL) headers not found. 
Please set CPPFLAGS appropriately or you can specify the location of the SDL installation using the SDL environment variable (e.g. ./configure SDL=$HOME/SDL)]))
                              ])
	RCSS_KEROSIN_IF_ELSE([
                              rcss_tmp="$LDFLAGS"
                              LDFLAGS="$LDFLAGS -lSDL"
                              AC_LINK_IFELSE([#include <SDL/SDL.h>
                                              int main() { return SDL_Init(0); }],,
        		      RCSS_BUILD_KEROSIN_ERROR([The SDL library (libSDL.a or libSDL.so) cannot be found. 
Please set LDFLAGS appropriately or you can specify the location of the SDL installation using the SDL environment variable (e.g. ./configure SDL=$HOME/SDL)]))
                              LDFLAGS="$rcss_tmp"
                              ])


]) # RCSS_CHECK_SDL

# RCSS_CHECK_SLANG
#-----------------------------------------------------------------------------
AC_DEFUN(RCSS_CHECK_SLANG, [
	RCSS_KEROSIN_IF_ELSE([
                              rcss_tmp="$LDFLAGS"
                              LDFLAGS="$LDFLAGS -lslang"
                              AC_LINK_IFELSE([int main() { return 0; }],,
        		      RCSS_BUILD_KEROSIN_ERROR([The S-Lang library (libslang.a or libslang.so) cannot be found. 
Please set LDFLAGS appropriately.]))
                              LDFLAGS="$rcss_tmp"
	])
]) # RCSS_CHECK_SLANG

# RCSS_BUILD_SOUNDSYSTEMFMOD
#	defines a conditional BUILD_SOUNDSYSTEMFMOD if the required header
#	and library exists. Up to date, there is no version check for the 
#	fmod library.
#-----------------------------------------------------------------------------
AC_DEFUN(RCSS_BUILD_SOUNDSYSTEMFMOD, [
	AC_CHECK_HEADER(fmod/fmod.h, 
			[rcss_soundsystemfmod="true"], 
			[rcss_soundsystemfmod="false" &&
			 AC_MSG_WARN([FMOD Sound module will not be built])])
	if test $rcss_soundsystemfmod = "true"; then
		AC_MSG_CHECKING([if linking against libfmod is succeeds])
		rcss_tmp="$LDFLAGS"
		LDFLAGS="$LDFLAGS -lfmod"
 		AC_LINK_IFELSE([#include <fmod/fmod.h>
				int main() { return FSOUND_GetVolume(0); }],
				[rcss_soundsystemfmod="true" && AC_MSG_RESULT([yes])],
				[rcss_soundsystemfmod="false" && 
				 AC_MSG_RESULT([no]) &&
				 AC_MSG_WARN([there should be a link in one of your libdirs from libfmod.so to libfmod-X.YZ.so. If the link exists, try setting the LDFLAGS environment variable appropriately.])])
		LDFLAGS="$rcss_tmp"
	fi		
	AM_CONDITIONAL(BUILD_SOUNDSYSTEMFMOD, test x$rcss_soundsystemfmod = xtrue)
]) # RCSS_BUILD_SOUNDSYSTEMFMOD


# RCSS_BUILD_KEROSIN
# 	defines preprocessor symbol HAVE_KEROSIN_H if kerosin can be build
#       set automake conditional BUILD_KEROSIN to true if kerosin can be build
#-----------------------------------------------------------------------------
AC_DEFUN(RCSS_BUILD_KEROSIN, [
	AC_REQUIRE([RCSS_BUILD_KEROSIN_INIT])
	AC_REQUIRE([RCSS_CHECK_GL])
 	AC_REQUIRE([RCSS_PATH_FREETYPE])
	AC_REQUIRE([RCSS_CHECK_SDL])
	AC_REQUIRE([RCSS_CHECK_DEVIL])
	AC_REQUIRE([RCSS_CHECK_SLANG])
    	AC_MSG_CHECKING([if libkerosin will be build])
 	AC_MSG_RESULT([$rcss_build_kerosin])
	RCSS_KEROSIN_IF_ELSE([AC_DEFINE(HAVE_KEROSIN_H, 1, [Define to 1 if using the kerosin header])],[
 		AC_MSG_WARN([libkerosin will not be build. There will be no graphical output.])
    		AC_MSG_NOTICE([the list of libraries required for kerosin can be found in the documentation])
 	])
	AM_CONDITIONAL(BUILD_KEROSIN, test x$rcss_build_kerosin = xyes)
]) # RCSS_BUILD_KEROSIN

# RCSS_BUILD_KEROSIN_INIT
# 	set rcss_build_kerosin to 'yes'
#-----------------------------------------------------------------------------
AC_DEFUN(RCSS_BUILD_KEROSIN_INIT, [
	# --enable-kerosin
	AC_ARG_ENABLE(kerosin,
		AC_HELP_STRING([--enable-kerosin=@<:@yes|no@:>@],       
	 		       [whether to compile libkerosin (default is yes)]),
		    [rcss_build_kerosin="$enableval"],
		    [rcss_build_kerosin=yes]
	)
	if test "$rcss_build_kerosin" = yes; then
		AC_MSG_NOTICE([Checking prerequisites for kerosin (building kerosin is optional)])
		AC_PATH_X
	fi
]) # RCSS_BUILD_KEROSIN_INIT

# RCSS_BUILD_KEROSIN_ERROR
# 	print a warning and set rcss_build_kerosin to 'no'
#-----------------------------------------------------------------------------
AC_DEFUN(RCSS_BUILD_KEROSIN_ERROR, [
	AC_MSG_WARN($1)
	rcss_build_kerosin=no
]) # RCSS_BUILD_KEROSIN_ERROR

# RCSS_KEROSIN_IF_ELSE
# 	if rcss_build_kerosin is 'yes', execute the if part (first parameter)
#	if rcss_build_kerosin is unequal to 'yes', execute the else part (2nd)
#-----------------------------------------------------------------------------
AC_DEFUN(RCSS_KEROSIN_IF_ELSE, [
	if test "$rcss_build_kerosin" = yes; then
		:
		$1
	else
		:
		$2
	fi
]) # RCSS_KEROSIN_IF_ELSE

