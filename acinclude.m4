dnl ---------------------------------------------------------------------------
dnl the wxWidgets macros were copied and adapted from the 'hack4u' project
dnl http://games.technoplaza.net/
dnl ---------------------------------------------------------------------------

dnl ---------------------------------------------------------------------------
dnl RCSS_OPTIONS_WXCONFIG
dnl
dnl adds support for --wx-prefix, --wx-exec-prefix and --wx-config
dnl command line options
dnl ---------------------------------------------------------------------------

AC_DEFUN([RCSS_OPTIONS_WXCONFIG],
[
   AC_ARG_WITH(wx-prefix, [  --with-wx-prefix=PREFIX   Prefix where wxWindows is installed (optional)],
               wx_config_prefix="$withval", wx_config_prefix="")
   AC_ARG_WITH(wx-exec-prefix,[  --with-wx-exec-prefix=PREFIX Exec prefix where wxWindows is installed (optional)],
               wx_config_exec_prefix="$withval", wx_config_exec_prefix="")
   AC_ARG_WITH(wx-config,[  --with-wx-config=CONFIG   wx-config script to use (optional)],
               wx_config_name="$withval", wx_config_name="")
])


dnl ---------------------------------------------------------------------------
dnl RCSS_CHECK_WXWIDGETS(VERSION, [ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]])
dnl
dnl Test for wxWindows, and define WX_C*FLAGS, WX_LIBS and WX_LIBS_STATIC
dnl (the latter is for static linking against wxWindows). Set WX_CONFIG_NAME
dnl environment variable to override the default name of the wx-config script
dnl to use. Set WX_CONFIG_PATH to specify the full path to wx-config - in this
dnl case the macro won't even waste time on tests for its existence.
dnl ---------------------------------------------------------------------------

dnl
dnl Get the cflags and libraries from the wx-config script
dnl
AC_DEFUN([RCSS_CHECK_WXWIDGETS],
[
  # --with-wxWidgets
  AC_ARG_WITH(wxWidgets, AC_HELP_STRING([--with-wxWidgets=@<:@yes|no@:>@],
	 		       [whether to use wxWidgets (default is yes)]),
	 		       [], [with_wxWidgets=yes] )

  if test x$with_wxWidgets != xno; then

  dnl do we have wx-config name: it can be wx-config or wxd-config or ...
  if test x${WX_CONFIG_NAME+set} != xset ; then
     WX_CONFIG_NAME=wx-config
  fi
  if test "x$wx_config_name" != x ; then
     WX_CONFIG_NAME="$wx_config_name"
  fi

  dnl deal with optional prefixes
  if test x$wx_config_exec_prefix != x ; then
     wx_config_args="$wx_config_args --exec-prefix=$wx_config_exec_prefix"
     WX_LOOKUP_PATH="$wx_config_exec_prefix/bin"
  fi
  if test x$wx_config_prefix != x ; then
     wx_config_args="$wx_config_args --prefix=$wx_config_prefix"
     WX_LOOKUP_PATH="$WX_LOOKUP_PATH:$wx_config_prefix/bin"
  fi

  dnl don't search the PATH if WX_CONFIG_NAME is absolute filename
  if test -x "$WX_CONFIG_NAME" ; then
     AC_MSG_CHECKING(for wx-config)
     WX_CONFIG_PATH="$WX_CONFIG_NAME"
     AC_MSG_RESULT($WX_CONFIG_PATH)
  else
     AC_PATH_PROG(WX_CONFIG_PATH, $WX_CONFIG_NAME, no, "$WX_LOOKUP_PATH:$PATH")
  fi

  if test "$WX_CONFIG_PATH" != "no" ; then
    WX_VERSION=""
    no_wx=""

    min_wx_version=ifelse([$1], ,2.2.1,$1)
    AC_MSG_CHECKING(for wxWindows version >= $min_wx_version)

    WX_CONFIG_WITH_ARGS="$WX_CONFIG_PATH $wx_config_args"

    WX_VERSION=`$WX_CONFIG_WITH_ARGS --version`
    wx_config_major_version=`echo $WX_VERSION | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\1/'`
    wx_config_minor_version=`echo $WX_VERSION | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\2/'`
    wx_config_micro_version=`echo $WX_VERSION | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\3/'`

    wx_requested_major_version=`echo $min_wx_version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\1/'`
    wx_requested_minor_version=`echo $min_wx_version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\2/'`
    wx_requested_micro_version=`echo $min_wx_version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\3/'`

    wx_ver_ok=""
    if test $wx_config_major_version -gt $wx_requested_major_version; then
      wx_ver_ok=yes
    else
      if test $wx_config_major_version -eq $wx_requested_major_version; then
         if test $wx_config_minor_version -gt $wx_requested_minor_version; then
            wx_ver_ok=yes
         else
            if test $wx_config_minor_version -eq $wx_requested_minor_version; then
               if test $wx_config_micro_version -ge $wx_requested_micro_version; then
                  wx_ver_ok=yes
               fi
            fi
         fi
      fi
    fi

    if test "x$wx_ver_ok" = x ; then
      no_wx=yes
    else
      WX_LIBS=`$WX_CONFIG_WITH_ARGS --libs gl std`
      WX_LIBS_STATIC=`$WX_CONFIG_WITH_ARGS --static --libs gl std`

      dnl starting with version 2.2.6 wx-config has --cppflags argument
      wx_has_cppflags=""
      if test $wx_config_major_version -gt 2; then
        wx_has_cppflags=yes
      else
        if test $wx_config_major_version -eq 2; then
           if test $wx_config_minor_version -gt 2; then
              wx_has_cppflags=yes
           else
              if test $wx_config_minor_version -eq 2; then
                 if test $wx_config_micro_version -ge 6; then
                    wx_has_cppflags=yes
                 fi
              fi
           fi
        fi
      fi

      if test "x$wx_has_cppflags" = x ; then
         dnl no choice but to define all flags like CFLAGS
         WX_CFLAGS=`$WX_CONFIG_WITH_ARGS --cflags`
         WX_CPPFLAGS=$WX_CFLAGS
         WX_CXXFLAGS=$WX_CFLAGS

         WX_CFLAGS_ONLY=$WX_CFLAGS
         WX_CXXFLAGS_ONLY=$WX_CFLAGS
      else
         dnl we have CPPFLAGS included in CFLAGS included in CXXFLAGS
         WX_CPPFLAGS=`$WX_CONFIG_WITH_ARGS --cppflags`
         WX_CXXFLAGS=`$WX_CONFIG_WITH_ARGS --cxxflags`
         WX_CFLAGS=`$WX_CONFIG_WITH_ARGS --cflags`

         WX_CFLAGS_ONLY=`echo $WX_CFLAGS | sed "s@^$WX_CPPFLAGS *@@"`
         WX_CXXFLAGS_ONLY=`echo $WX_CXXFLAGS | sed "s@^$WX_CFLAGS *@@"`
      fi
    fi

    if test "x$no_wx" = x ; then
       AC_MSG_RESULT(yes (version $WX_VERSION))
       ifelse([$2], , :, [$2])
    else
       if test "x$WX_VERSION" = x; then
	  dnl no wx-config at all
	  AC_MSG_RESULT(no)
       else
	  AC_MSG_RESULT(no (version $WX_VERSION is not new enough))
       fi

       WX_CFLAGS=""
       WX_CPPFLAGS=""
       WX_CXXFLAGS=""
       WX_LIBS=""
       WX_LIBS_STATIC=""
       ifelse([$3], , :, [$3])
    fi
  fi
  else
    AC_MSG_NOTICE(do not use wxWidgets)
  fi
  AC_SUBST(WX_CPPFLAGS)
  AC_SUBST(WX_CFLAGS)
  AC_SUBST(WX_CXXFLAGS)
  AC_SUBST(WX_CFLAGS_ONLY)
  AC_SUBST(WX_CXXFLAGS_ONLY)
  AC_SUBST(WX_LIBS)
  AC_SUBST(WX_LIBS_STATIC)
  AC_SUBST(WX_VERSION)
])

# RCSS_PATH_RUBY
# Substitutes: @RUBY@		ruby command
#              @RUBY_CPPFLAGS@  additional CPPFLAGS to use to get ruby.h
#	       @RUBY_LDFLAGS@   additional LDFLAGS to use when linking libruby
#	       @RUBY_LDADD@     additional libraries libruby depends on
#
# original version from the Swig list (major changes by me /oliver)
# at http://mailman.cs.uchicago.edu/mailman/listinfo/swig
#-----------------------------------------------------------------------------
AC_DEFUN([RCSS_PATH_RUBY], [
	# --with-ruby=PATH
	AC_ARG_WITH(ruby,
		AC_HELP_STRING([--with-ruby=PATH],
	 		       [path to Ruby interpreter (default is ruby)]),
		    [ruby="$withval"],
		    [ruby="ruby"]
	)
        AC_PATH_PROGS(RUBY,[$ruby ruby],no)
	# Test ruby interpreter
	if test $RUBY = no; then
	   AC_PATH_PROGS(RUBY,[$ruby ruby1.9],no)
	   if test $RUBY = no; then
	      AC_PATH_PROGS(RUBY,[$ruby ruby1.8],no)
	      if test $RUBY = no; then
	      	 AC_MSG_ERROR(Could not find Ruby Interpreter.  Please use --with-ruby option.)
	      fi
	   fi
	fi

	# check ruby headers
	AC_CHECK_HEADER(ruby.h,
			[RUBY_CPPFLAGS=''],
			[unset ac_cv_header_ruby_h
		         ruby_tmp=$CPPFLAGS
                         ruby_includedir=`$RUBY -rrbconfig -e "print Config::CONFIG[['archdir']]"`
                         AC_MSG_NOTICE(trying again in $ruby_includedir)
		         RUBY_CPPFLAGS="-I$ruby_includedir"
                         CPPFLAGS="$ruby_tmp $RUBY_CPPFLAGS"
                         AC_CHECK_HEADER(ruby.h, [], [AC_MSG_ERROR(check your ruby installation)])
                         CPPFLAGS=$ruby_tmp
                        ]
        		)

        # set ruby ldflags
	AC_MSG_CHECKING(ruby library directory)
	ruby_libdir=`$RUBY -rrbconfig -e "print Config::CONFIG[['archdir']]"`
	ruby_lib=`$RUBY -rrbconfig -e "print Config::CONFIG[['LIBRUBYARG']]"`
	AC_MSG_RESULT($ruby_libdir)
	RUBY_LDFLAGS="-L$ruby_libdir $ruby_lib"

	# set ruby additional libraries
	AC_MSG_CHECKING(libraries ruby depends on)
	RUBY_LDADD=`$RUBY -rrbconfig -e "print Config::CONFIG[['LIBS']]"`
	AC_MSG_RESULT($RUBY_LDADD)

	AC_MSG_CHECKING([for libruby])
	rcss_tmp="$LDFLAGS"
	LDFLAGS="$LDFLAGS $RUBY_LDFLAGS"
 	AC_LINK_IFELSE([int main(int argc, char **argv) { return 0; }],
		       [AC_MSG_RESULT([yes])],
		       [AC_MSG_RESULT([no])
		        AC_MSG_ERROR([libruby cannot be found. If you compile ruby on your own,
	make sure to configure ruby with '--enable-shared' to create the library.])])
	LDFLAGS="$rcss_tmp"

	# Substitute Makefile Vars.
	AC_SUBST(RUBY)
	AC_SUBST(RUBY_CPPFLAGS)
	AC_SUBST(RUBY_LDFLAGS)
	AC_SUBST(RUBY_LDADD)
]) # RCSS_PATH_RUBY

AC_DEFUN([RCSS_CHECK_RUBY_VERSION], [
	AC_MSG_CHECKING([ruby version])
	AC_REQUIRE([RCSS_PATH_RUBY])
	MAJOR="$1"
	RUBY_MAJOR=`$RUBY -rrbconfig -e "print Config::CONFIG[['MAJOR']]"`
	test -z "$1" && MAJOR="$RUBY_MAJOR"
	MINOR="$2"
	RUBY_MINOR=`$RUBY -rrbconfig -e "print Config::CONFIG[['MINOR']]"`
	test -z "$2" && MINOR="$RUBY_MINOR"
	TEENY="$3"
	RUBY_TEENY=`$RUBY -rrbconfig -e "print Config::CONFIG[['TEENY']]"`
	test -z "$3" && TEENY="$RUBY_TEENY"
	AC_MSG_RESULT($RUBY_MAJOR.$RUBY_MINOR.$RUBY_TEENY)
	if test \( "$RUBY_MAJOR" -lt "$MAJOR" \) -o \
		\( "$RUBY_MAJOR" -eq "$MAJOR" -a "$RUBY_MINOR" -lt "$MINOR" \) -o \
		\( "$RUBY_MAJOR" -eq "$MAJOR" -a "$RUBY_MINOR" -eq "$MINOR" -a "$RUBY_TEENY" -lt "$TEENY" \); then
		AC_MSG_ERROR([Your ruby is too old. Use at least ruby-$MAJOR.$MINOR.$TEENY])
	fi
]) # RCSS_CHECK_RUBY_VERSION

# RCSS_CHECK_ODE
#-----------------------------------------------------------------------------
AC_DEFUN([RCSS_CHECK_ODE], [
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
 	AC_LINK_IFELSE([int main(int argc, char **argv) { return 0; }],
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
AC_DEFUN([RCSS_PATH_FREETYPE], [
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
			rcss_freetype_libadd=`$FREETYPE --libs`
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
#		     @GL_LIBADD@ with linker options for libraries linking libgl
#                    @GL_LDFLAGS@ with linker flags for libraries linking libgl
#-----------------------------------------------------------------------------
AC_DEFUN([RCSS_CHECK_GL], [
    AC_CHECK_HEADERS([OpenGL/gl.h OpenGL/glu.h], [iamamac=yes], [iamamac=no])
    AC_MSG_CHECKING([if I am a Mac])
    AM_CONDITIONAL(BUNDLE_SUPPORT, test x$iamamac = xyes)
    AC_SUBST(bundle_support, $iamamac)
    if test "$iamamac" = yes; then
       AC_MSG_RESULT([yes])
       RCSS_KEROSIN_IF_ELSE([
	# subst'ing the directory where the prepocessor finds gl.h
	# JAN: the awk command may have to be state more precisely, if problems with distris occur
        GLDIR=`echo "#include <OpenGL/gl.h>" | cpp -M | awk '/gl.h/ { print @S|@2 }'`
        GLDIR=`dirname "$GLDIR"`
        AC_SUBST([GLDIR], [$GLDIR])
	rcss_GL_LIBADD=""
	rcss_GL_LDFLAGS="-framework GLUT -framework OpenGL"
	AC_CHECK_HEADERS([GLUT/glut.h],[have_glut=yes],[have_glut=no])
	AM_CONDITIONAL(BUILD_GLUT, test x$have_glut = xyes)
	# checking if linking against libGL succeeds
	RCSS_KEROSIN_IF_ELSE([
		AC_MSG_CHECKING([if linking against libGL succeeds])
		rcss_tmp="$LDFLAGS"
		LDFLAGS="$LDFLAGS $rcss_GL_LIBADD $rcss_GL_LDFLAGS"
 		AC_LINK_IFELSE([#include <OpenGL/gl.h>
				int main(int argc, char **argv) { glColor3f(0,0,0); }],
				[AC_MSG_RESULT([yes])],
				[AC_MSG_RESULT([no])
				 RCSS_BUILD_KEROSIN_ERROR([to build libkerosin, set LDFLAGS so that libGL can be found])])
		LDFLAGS="$rcss_tmp"
		if test "$have_glut" = yes; then
			AC_MSG_CHECKING([if linking against libglut succeeds])
			LDFLAGS="$LDFLAGS $rcss_GL_LIBADD $rcss_GL_LDFLAGS"
	 		AC_LINK_IFELSE([#include <GLUT/glut.h>
					int main(int argc, char **argv) { glutMainLoop(); }],
					[AC_MSG_RESULT([yes])],
					[AC_MSG_RESULT([no])
					 RCSS_BUILD_KEROSIN_ERROR([to build libkerosin, set LDFLAGS so that libglut or can be found])
					 ])
			LDFLAGS="$rcss_tmp"
			])
		fi
    ])
    else
       AC_MSG_RESULT([sorry])
       RCSS_KEROSIN_IF_ELSE([
	# check for OpenGL location and used extensions
    		AC_CHECK_HEADERS([GL/gl.h GL/glu.h],,
                                RCSS_BUILD_KEROSIN_ERROR([not all required OpenGL headers could not be found. Please specify the location  of the OpenGL header directory using the CPPFLAGS environment variable]))
    		AC_CHECK_HEADERS([GL/glut.h],[have_glut=yes],[have_glut=no])
    		AM_CONDITIONAL(BUILD_GLUT, test x$have_glut = xyes)
		RCSS_KEROSIN_IF_ELSE([
			AC_CHECK_HEADERS([GL/glx.h], AC_SUBST([GLTARGET], [x]),
                        		 AC_CHECK_HEADERS([GL/wglext.h],
                                   	 AC_SUBST([GLTARGET], [win32]),
                                         RCSS_BUILD_KEROSIN_ERROR([could not find
 X nor MS Windows GL extensions]),
 				          [#include <GL/gl.h>]),
                          [#include <GL/gl.h>])

	# subst'ing the directory where the prepocessor finds gl.h
	# JAN: the awk command may have to be state more precisely, if problems with distris occur
        GLDIR=`echo "#include <GL/gl.h>" | cpp -M | awk '/gl.h/ { print @S|@2 }'`
        GLDIR=`dirname "$GLDIR"`
        AC_SUBST([GLDIR], [$GLDIR])

	# checking if linking against libGL succeeds
	RCSS_KEROSIN_IF_ELSE([
		AC_MSG_CHECKING([if linking against libGL succeeds])
		rcss_GL_LIBADD="-lGL"
		rcss_GL_LDFLAGS=""
		rcss_tmp="$LDFLAGS"
		LDFLAGS="$LDFLAGS $rcss_GL_LIBADD $rcss_GL_LDFLAGS"
 		AC_LINK_IFELSE([#include <GL/gl.h>
				int main(int argc, char **argv) { glColor3f(0,0,0); }],
				[AC_MSG_RESULT([yes])],
				[AC_MSG_RESULT([no])
				 RCSS_BUILD_KEROSIN_ERROR([to build libkerosin, set LDFLAGS so that libGL can be found])])
		LDFLAGS="$rcss_tmp"

		AC_MSG_CHECKING([if linking against libGLU succeeds])
		rcss_GL_LIBADD="-lGL -lGLU"
		rcss_GL_LDFLAGS=""
		LDFLAGS="$LDFLAGS $rcss_GL_LIBADD $rcss_GL_LDFLAGS"
 		AC_LINK_IFELSE([#include <GL/glu.h>
				int main(int argc, char **argv) { gluPickMatrix(0,0,0,0,0); }],
				[AC_MSG_RESULT([yes])],
				[AC_MSG_RESULT([no])
				 RCSS_BUILD_KEROSIN_ERROR([to build libkerosin, set LDFLAGS so that libGLU can be found])])
		LDFLAGS="$rcss_tmp"

		if test "$have_glut" = yes; then
			AC_MSG_CHECKING([if linking against libglut succeeds])
			rcss_GL_LIBADD="-lGL -lGLU -lglut"
			rcss_GL_LDFLAGS=""
			LDFLAGS="$LDFLAGS $rcss_GL_LIBADD $rcss_GL_LDFLAGS"
	 		AC_LINK_IFELSE([#include <GL/glut.h>
					int main(int argc, char **argv) { glutMainLoop(); }],
					[AC_MSG_RESULT([yes])],
					[AC_MSG_RESULT([no])
					 RCSS_BUILD_KEROSIN_ERROR([to build libkerosin, set LDFLAGS so that libglut or can be found])
					 ])
			LDFLAGS="$rcss_tmp"
			])
		fi
    ])
  ])
  fi
  AC_SUBST([GL_LIBADD], [$rcss_GL_LIBADD])
  AC_SUBST([GL_LDFLAGS], [$rcss_GL_LDFLAGS])
]) # RCSS_CHECK_GL

# RCSS_CHECK_DEVIL
# 	if rcss_build_kerosin is (still) set to 'yes', check for the DEVIL
#	headers and libraries.
#	If DEVIL headers or libraries can not be found, building kerosin will
#	be disabled.
#	Substitutes: @IL_LIBADD@ with linker options for libraries linking libIL
#                    @IL_LDFLAGS@ with linker flags for libraries linking libIL
# Defines: HAVE_IL_IL_H if the IL/il.h header file is present
#-----------------------------------------------------------------------------
AC_DEFUN([RCSS_CHECK_DEVIL], [
	rcss_IL_LIBADD=""
	rcss_IL_LDFLAGS=""
	AC_ARG_VAR(DEVIL, [location of DevIL installation])
	if test $DEVIL; then
		CPPFLAGS="$CPPFLAGS -I$DEVIL/include"
		LDFLAGS="$LDFLAGS -L$DEVIL/lib"
	fi
	RCSS_KEROSIN_IF_ELSE([
                              AC_CHECK_HEADERS([IL/il.h],,
                              RCSS_BUILD_KEROSIN_ERROR([DevIL headers not found.
Please set CPPFLAGS appropriately or you can specify the location of the DevIL installation using the DEVIL environment variable (e.g. ./configure DEVIL=$HOME/DevIL)]))
                              ])
	RCSS_KEROSIN_IF_ELSE([
                              rcss_tmp="$LDFLAGS"
			      rcss_IL_LIBADD="-lIL"
			      LDFLAGS="$LDFLAGS $rcss_IL_LDFLAGS $rcss_IL_LIBADD"
                              AC_LINK_IFELSE([#include <IL/il.h>
#include <stdarg.h> /* _vsnprintf may be undefined (and it is needed by libIL) */
extern "C" int _vsnprintf(char *str, size_t size, const char *format, va_list ap) { return 0;}
int main(int argc, char **argv) { ilInit(); return 0; }], rcss_tmp_il_ok="yes",rcss_tmp_il_ok="no")
    	     	   	      if test $rcss_tmp_il_ok = "no"; then
			        if test $iamamac = "yes"; then
				  rcss_IL_LIBADD=""
			      	  rcss_IL_LDFLAGS="-framework IL"
				  LDFLAGS="$rcss_tmp $rcss_IL_LDFLAGS"
				  AC_LINK_IFELSE([#include <IL/il.h>
#include <stdarg.h> /* _vsnprintf may be undefined (and it is needed by libIL) */
extern "C" int _vsnprintf(char *str, size_t size, const char *format, va_list ap) { return 0;}
int main(int argc, char **argv) { ilInit(); return 0; }], rcss_tmp_il_ok="yes",rcss_tmp_il_ok="no")
			        fi
			      fi
			      if test $rcss_tmp_il_ok = "no"; then
          		        RCSS_BUILD_KEROSIN_ERROR([The DevIL library (libIL.a or libIL.so) cannot be found.
Please set LDFLAGS appropriately or you can specify the location of the DevIL installation using the DEVIL environment variable (e.g. ./configure DEVIL=$HOME/DevIL)])
       	   	   	      fi
                              LDFLAGS="$rcss_tmp"
                              ])
   AC_SUBST([IL_LIBADD],[$rcss_IL_LIBADD])
   AC_SUBST([IL_LDFLAGS],[$rcss_IL_LDFLAGS])
]) # RCSS_CHECK_DEVIL

# RCSS_CHECK_SDL
# 	if rcss_build_kerosin is (still) set to 'yes', check for the SDL
#	headers and libraries.
#	If SDL headers or libraries can not be found, building kerosin will be
#       disabled.
#	Substitutes: @SDL_LIBADD@ with linker options for libraries linking libSDL
#                    @SDL_LDFLAGS@ with linker flags for libraries linking libSDL
#		     @SDL_CPPFLAGS@ with compiler flags for using libSDL
#-----------------------------------------------------------------------------
AC_DEFUN([RCSS_CHECK_SDL], [
	AC_PATH_PROG(SDLCONFIG, [sdl-config], no, [$SDL/bin:$PATH])
	AC_ARG_VAR(SDL, [location of SDL installation])
	rcss_SDL_LIBADD=""
	rcss_SDL_LDFLAGS=""
	rcss_SDL_CPPFLAGS=""
	if test "$SDLCONFIG" !=	"no"; then
	   rcss_SDL_CPPFLAGS="`$SDLCONFIG --cflags`"
	   rcss_SDL_LDFLAGS="`$SDLCONFIG --libs`"
	else
	  if test $SDL; then
	    rcss_SDL_CPPFLAGS="-I$SDL/include/SDL"
	    rcss_SDL_LDFLAGS="-L$SDL/lib -lSDL"
	  fi
	fi
	rcss_tmp_CPPFLAGS=$CPPFLAGS
	rcss_tmp_LDFLAGS=$LDFLAGS
	CPPFLAGS="$CPPFLAGS $rcss_SDL_CPPFLAGS"
	LDFLAGS="$LDFLAGS $rcss_SDL_LDFLAGS $rcss_LIBADD"
	RCSS_KEROSIN_IF_ELSE([
                              AC_CHECK_HEADER([SDL.h],,
                              RCSS_BUILD_KEROSIN_ERROR([Simple DirectMedia Layer (SDL) headers not found.
Please set CPPFLAGS appropriately or you can specify the location of the SDL installation using the SDL environment variable (e.g. ./configure SDL=$HOME/SDL)]))
                              ])
	RCSS_KEROSIN_IF_ELSE([
                              AC_LINK_IFELSE([#include <SDL.h>
                                              int main(int argc, char **argv) { return SDL_Init(0); }],,
        		      RCSS_BUILD_KEROSIN_ERROR([The SDL library (libSDL.a or libSDL.so) cannot be found.
Please set LDFLAGS appropriately or you can specify the location of the SDL installation using the SDL environment variable (e.g. ./configure SDL=$HOME/SDL)]))
                              ])
	CPPFLAGS=$rcss_tmp_CPPFLAGS
	LDFLAGS=$rcss_tmp_LDFLAGS
        AC_SUBST([SDL_LIBADD], [$rcss_SDL_LIBADD])
        AC_SUBST([SDL_LDFLAGS], [$rcss_SDL_LDFLAGS])
        AC_SUBST([SDL_CPPFLAGS], [$rcss_SDL_CPPFLAGS])
]) # RCSS_CHECK_SDL

# RCSS_CHECK_SLANG
#-----------------------------------------------------------------------------
AC_DEFUN([RCSS_CHECK_SLANG], [
	AC_MSG_CHECKING([for the S-Lang library])
        if test $iamamac = "yes"; then
	    AC_MSG_RESULT([not needed])
	else
	    RCSS_KEROSIN_IF_ELSE([
                              rcss_tmp="$LDFLAGS"
                              LDFLAGS="$LDFLAGS -lslang"
                              AC_LINK_IFELSE([int main(int argc, char **argv) { return 0; }],,AC_MSG_RESULT(no)
        		      RCSS_BUILD_KEROSIN_ERROR([The S-Lang library (libslang.a or libslang.so) cannot be found.
Please set LDFLAGS appropriately.]))
                              LDFLAGS="$rcss_tmp"
	    ])
	    AC_MSG_RESULT(yes)
     	fi
]) # RCSS_CHECK_SLANG

# RCSS_BUILD_SOUNDSYSTEMFMOD
#	defines a conditional BUILD_SOUNDSYSTEMFMOD if the required header
#	and library exists. Up to date, there is no version check for the
#	fmod library.
#-----------------------------------------------------------------------------
AC_DEFUN([RCSS_BUILD_SOUNDSYSTEMFMOD], [
	AC_CHECK_HEADER(fmod/fmod.h,
			[rcss_soundsystemfmod="true"],
			[rcss_soundsystemfmod="false" &&
			 AC_MSG_WARN([FMOD Sound module will not be built])])
	if test $rcss_soundsystemfmod = "true"; then
		AC_MSG_CHECKING([if linking against libfmod is succeeds])
		rcss_tmp="$LDFLAGS"
		LDFLAGS="$LDFLAGS -lfmod"
 		AC_LINK_IFELSE([#include <fmod/fmod.h>
				int main(int argc, char **argv) { return FSOUND_GetVolume(0); }],
				[rcss_soundsystemfmod="true" && AC_MSG_RESULT([yes])],
				[rcss_soundsystemfmod="false" &&
				 AC_MSG_RESULT([no]) &&
				 AC_MSG_WARN([there should be a link in one of your libdirs from libfmod.so to libfmod-X.YZ.so. If the link exists, try setting the LDFLAGS environment variable appropriately.])])
		LDFLAGS="$rcss_tmp"
	fi
	AM_CONDITIONAL(BUILD_SOUNDSYSTEMFMOD, test x$rcss_soundsystemfmod = xtrue)
]) # RCSS_BUILD_SOUNDSYSTEMFMOD


# RCSS_BUILD_KEROSIN_INTERNAL
# 	defines preprocessor symbol HAVE_KEROSIN_KEROSIN_H if kerosin can be build
#       set automake conditional BUILD_KEROSIN to true if kerosin can be build
#-----------------------------------------------------------------------------
AC_DEFUN([RCSS_BUILD_KEROSIN_INTERNAL], [
	RCSS_CHECK_GL
 	RCSS_PATH_FREETYPE
	RCSS_CHECK_SDL
	RCSS_CHECK_DEVIL
	RCSS_CHECK_SLANG
    	AC_MSG_CHECKING([if libkerosin will be build])
 	AC_MSG_RESULT([$rcss_build_kerosin])
	RCSS_KEROSIN_IF_ELSE([AC_DEFINE(HAVE_KEROSIN_KEROSIN_H, 1, [Define to 1 if using the kerosin header])],[
 		AC_MSG_ERROR([libkerosin cannot be build without all required libraries.])
    		AC_MSG_ERROR([the list of libraries required for kerosin can be found in the documentation])
 	])
]) # RCSS_BUILD_KEROSIN_INTERNAL

# RCSS_BUILD_KEROSIN
# 	set rcss_build_kerosin to 'yes'
#-----------------------------------------------------------------------------
AC_DEFUN([RCSS_BUILD_KEROSIN], [
	# --enable-kerosin
	AC_ARG_ENABLE(kerosin,
		AC_HELP_STRING([--enable-kerosin=@<:@yes|no@:>@],
	 		       [whether to compile libkerosin (default is yes)]),
		    [rcss_build_kerosin="$enableval"],
		    [rcss_build_kerosin=yes]
	)
	if test "$rcss_build_kerosin" = yes; then
		AC_MSG_NOTICE([Checking prerequisites for kerosin...])
		AC_PATH_X
		RCSS_BUILD_KEROSIN_INTERNAL
	fi
	AM_CONDITIONAL(BUILD_KEROSIN, test x$rcss_build_kerosin = xyes)
	if test "$rcss_build_kerosin" = no; then
		AC_MSG_NOTICE([libkerosin will not be build...])
		AC_MSG_NOTICE([...you can enable it using the --enable-kerosin flag])
	fi
]) # RCSS_BUILD_KEROSIN

# RCSS_BUILD_KEROSIN_ERROR
# 	print a warning and set rcss_build_kerosin to 'no'
#-----------------------------------------------------------------------------
AC_DEFUN([RCSS_BUILD_KEROSIN_ERROR], [
	AC_MSG_WARN($1)
	rcss_build_kerosin=no
]) # RCSS_BUILD_KEROSIN_ERROR

# RCSS_KEROSIN_IF_ELSE
# 	if rcss_build_kerosin is 'yes', execute the if part (first parameter)
#	if rcss_build_kerosin is unequal to 'yes', execute the else part (2nd)
#-----------------------------------------------------------------------------
AC_DEFUN([RCSS_KEROSIN_IF_ELSE], [
	if test "$rcss_build_kerosin" = yes; then
		:
		$1
	else
		:
		$2
	fi
]) # RCSS_KEROSIN_IF_ELSE

# RCSS_BUILD_SPADES_SERVER
# 	set rcss_build_spades to 'yes'
#-----------------------------------------------------------------------------
AC_DEFUN([RCSS_BUILD_SPADES_SERVER], [
	AC_ARG_VAR(SPADES, [location of the SPADES library])
	# --enable-spades
	AC_ARG_ENABLE(spades,
		AC_HELP_STRING([--enable-spades=@<:@yes|no@:>@],
	 		       [whether to compile the spades simulation engine server (default is no)]),
		    [rcss_build_spades="$enableval"],
		    [rcss_build_spades=no]
	)
	if test "$rcss_build_spades" = yes; then
		AC_MSG_NOTICE([Checking prerequisites for building the spades server...])
		RCSS_LIB_SPADES
	else
		AC_MSG_NOTICE([======================================================])
		AC_MSG_NOTICE([You have chosen not to build the SPADES Server module.])
	fi
	if test "$rcss_build_spades" = no; then
		AC_MSG_WARN([The SPADES server will not be build. To build this module,])
		AC_MSG_WARN([you have to configure again using the --enable-spades option.])
	fi
	AM_CONDITIONAL(BUILD_SPADES_SERVER, test x$rcss_build_spades = xyes)
]) # RCSS_BUILD_SPADES_SERVER

# RCSS_LIB_SPADES
# Checks for the spades library
# ---------------------------------------------------------
AC_DEFUN([RCSS_LIB_SPADES],
       [AS_VAR_PUSHDEF([rcss_lib_spades], [rcss_cv_lib_spades])dnl
	if test "$SPADES"; then
	    PATH="$PATH:$SPADES/bin"
	    LDFLAGS="$LDFLAGS -L$SPADES/lib"
	    CPPFLAGS="$CPPFLAGS -I$SPADES/include"
	fi
	AC_CACHE_CHECK(whether the spades library is available, rcss_cv_lib_spades,
               [AC_LANG_PUSH(C++)
                OLD_LDFLAGS="$LDFLAGS"
                LDFLAGS="$LDFLAGS -lspades"
                AC_LINK_IFELSE([@%:@include <spades/enginemain.hpp>
                                int main()
                                {
                                    spades::SimulationEngineMain( 0, NULL, NULL );
                                    return 0;
                                }],
                                [AS_VAR_SET(rcss_lib_spades, yes)],
                                [AS_VAR_SET(rcss_lib_spades, no)])
                LDFLAGS="$OLD_LDFLAGS"
                AC_LANG_POP(C++)
                ])
AS_VAR_POPDEF([rcss_lib_spades])dnl
])# RCSS_LIB_SPADES

# RCSS_LIBRARY_VERSION_INFO
#	define two variables for the library versions
# ---------------------------------------------------------
AC_DEFUN([RCSS_LIBRARY_VERSION_INFO], [
$1=$3
AC_SUBST([$1])
$2=`echo $3 | tr : .`
AC_SUBST([$2])
]) # RCSS_LIBRARY_VERSION_INFO

# RCSS_BOOST_REGEX([ACTION-IF-FOUND], [ACTION-IF-NOT-FOUND])
# ---------------------------------------------------------
# Checks for the boost regex library
AC_DEFUN([RCSS_BOOST_REGEX],
[AS_VAR_PUSHDEF([rcss_boost_regex], [rcss_cv_boost_regex])dnl
AC_CACHE_CHECK(if linking against boost_regex succeeds, rcss_cv_boost_regex,
               [AC_LANG_PUSH(C++)
                OLD_LDFLAGS="$LDFLAGS"
                LDFLAGS="$LDFLAGS -lboost_regex"
                AC_LINK_IFELSE([int main(int argc, char **argv) { return 0; }],
                               [AS_VAR_SET(rcss_boost_regex, yes)],
                               [AS_VAR_SET(rcss_boost_regex, no)])
                LDFLAGS="$OLD_LDFLAGS"
                AC_LANG_POP(C++)
                ])
AS_IF([test AS_VAR_GET(rcss_boost_regex) = yes], [$1], [$2])
AS_VAR_POPDEF([rcss_boost_regex])dnl
])# RCSS_BOOST_REGEX

# RCSS_BOOST_THREADS_LIB
# ---------------------------------------------------------
# Checks for the boost threads library under various names
# Substitutes: @BOOST_THREADS_LIB@
AC_DEFUN([RCSS_BOOST_THREADS_LIB], [
  AC_MSG_CHECKING(boost thread library)
  AC_LANG_PUSH(C++)
  OLDLDFLAGS="$LDFLAGS"
  LDFLAGS="$OLDLDFLAGS -lboost_thread"
  AC_LINK_IFELSE([int main(int argc, char **argv) { return 0; }],
                 [rcss_boost_threads_lib=-lboost_thread],
                 [LDFLAGS="$OLDLDFLAGS -lboost_thread-mt"
      AC_LINK_IFELSE([int main(int argc, char **argv) { return 0; }],
                   [rcss_boost_threads_lib=-lboost_thread-mt],
                   [  LDFLAGS="$OLDLDFLAGS -lboost_thread-gcc-mt"
          AC_LINK_IFELSE([int main(int argc, char **argv) { return 0; }],
                       [rcss_boost_threads_lib=-lboost_thread-gcc-mt],
		       [  LDFLAGS="$OLDLDFLAGS -lboost_thread-gcc"
              AC_LINK_IFELSE([int main(int argc, char **argv) { return 0; }],
                           [rcss_boost_threads_lib=-lboost_thread-gcc],
                           [rcss_boost_threads_lib=])
		        ])
                    ])
                ])
  LDFLAGS="$OLD_LDFLAGS"
  AC_LANG_POP(C++)
  BOOST_THREADS_LIB=$rcss_boost_threads_lib
  AC_MSG_RESULT($rcss_boost_threads_lib)
  AC_SUBST(BOOST_THREADS_LIB)
])# RCSS_BOOST_THREADS_LIBS
