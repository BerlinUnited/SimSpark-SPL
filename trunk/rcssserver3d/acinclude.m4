# SPARK_LIB_SALT
# Substitutes: @SALT_LDFLAGS@
#              @SALT_CPPFLAGS@
#-----------------------------------------------------------------------------
AC_DEFUN([SPARK_LIB_SALT], [
	# check for the salt-config script
	AC_PATH_PROG(SALT,[salt-config],[no])
	if test $SALT = "no"; then
		AC_MSG_ERROR([salt-config not found])
	fi
	# get the libs file
	AC_MSG_CHECKING([libsalt libs file])

	spark_salt_libflags=`$SALT --libs`
	AC_MSG_RESULT([$spark_salt_libflags])
	AC_SUBST(SALT_LDFLAGS, [$spark_salt_libflags])

	spark_salt_cppflags=`$SALT --cflags`
	AC_MSG_RESULT([$spark_salt_cppflags])
	AC_SUBST(SALT_CPPFLAGS, [$spark_salt_cppflags])
]) # SPARK_LIB_SALT

# SPARK_LIB_ZEITGEIST
# Substitutes: @ZEITGEIST_LDFLAGS@
#              @ZEITGEIST_CPPFLAGS@
#-----------------------------------------------------------------------------
AC_DEFUN([SPARK_LIB_ZEITGEIST], [
	# check for the zeitgeist-config script
	AC_PATH_PROG(ZEITGEIST,[zeitgeist-config],[no])
	if test $ZEITGEIST = "no"; then
		AC_MSG_ERROR([zeitgeist-config not found])
	fi
	# get the libs file
	AC_MSG_CHECKING([libzeitgeist libs file])

	spark_zeitgeist_libflags=`$ZEITGEIST --libs`
	AC_MSG_RESULT([$spark_zeitgeist_libflags])
	AC_SUBST(ZEITGEIST_LDFLAGS, [$spark_zeitgeist_libflags])

	spark_zeitgeist_cppflags=`$ZEITGEIST --cflags`
	AC_MSG_RESULT([$spark_zeitgeist_cppflags])
	AC_SUBST(ZEITGEIST_CPPFLAGS, [$spark_zeitgeist_cppflags])
]) # SPARK_LIB_ZEITGEIST

# SPARK_LIB_OXYGEN
# Substitutes: @OXYGEN_LDFLAGS@
#              @OXYGEN_CPPFLAGS@
#-----------------------------------------------------------------------------
AC_DEFUN([SPARK_LIB_OXYGEN], [
	# check for the oxygen-config script
	AC_PATH_PROG(OXYGEN,[oxygen-config],[no])
	if test $OXYGEN = "no"; then
		AC_MSG_ERROR([oxygen-config not found])
	fi
	# get the libs file
	AC_MSG_CHECKING([liboxygen libs file])

	spark_oxygen_libflags=`$OXYGEN --libs`
	AC_MSG_RESULT([$spark_oxygen_libflags])
	AC_SUBST(OXYGEN_LDFLAGS, [$spark_oxygen_libflags])

	spark_oxygen_cppflags=`$OXYGEN --cflags`
	AC_MSG_RESULT([$spark_oxygen_cppflags])
	AC_SUBST(OXYGEN_CPPFLAGS, [$spark_oxygen_cppflags])
]) # SPARK_LIB_OXYGEN

# SPARK_LIB_KEROSIN
# Substitutes: @KEROSIN_LDFLAGS@
#              @KEROSIN_CPPFLAGS@
#-----------------------------------------------------------------------------
AC_DEFUN([SPARK_LIB_KEROSIN], [
	# check for the kerosin-config script
	AC_PATH_PROG(KEROSIN,[kerosin-config],[no])
	if test $KEROSIN = "no"; then
		AC_MSG_ERROR([kerosin-config not found])
	fi
	# get the libs file
	AC_MSG_CHECKING([libkerosin libs file])

	spark_kerosin_libflags=`$KEROSIN --libs`
	AC_MSG_RESULT([$spark_kerosin_libflags])
	AC_SUBST(KEROSIN_LDFLAGS, [$spark_kerosin_libflags])

	spark_kerosin_cppflags=`$KEROSIN --cflags`
	AC_MSG_RESULT([$spark_kerosin_cppflags])
	AC_SUBST(KEROSIN_CPPFLAGS, [$spark_kerosin_cppflags])

	AC_DEFINE(HAVE_KEROSIN_KEROSIN_H, 1, [Define to 1 if using the kerosin header])
]) # SPARK_LIB_KEROSIN

# SPARK_LIB_SPARK
# Substitutes: @SPARK_LDFLAGS@
#              @SPARK_CPPFLAGS@
#-----------------------------------------------------------------------------
AC_DEFUN([SPARK_LIB_SPARK], [
	# check for the spark-config script
	AC_PATH_PROG(SPARK,[spark-config],[no])
	if test $SPARK = "no"; then
		AC_MSG_ERROR([spark-config not found])
	fi
	# get the libs file
	AC_MSG_CHECKING([libspark libs file])

	spark_libflags=`$SPARK --libs`
	AC_MSG_RESULT([$spark_libflags])
	AC_SUBST(SPARK_LDFLAGS, [$spark_libflags])

	spark_cppflags=`$SPARK --cflags`
	AC_MSG_RESULT([$spark_cppflags])
	AC_SUBST(SPARK_CPPFLAGS, [$spark_cppflags])
]) # SPARK_LIB_SPARK

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

# RCSS_PATH_FREETYPE
# Substitutes: @FREETYPE_CPPFLAGS@
# 	       @FREETYPE_LDFLAGS@
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
	        AC_PATH_PROGS(FREETYPE,[$freetype freetype-config],no)
		if test $FREETYPE = "no"; then
			AC_MSG_ERROR([need freetype2 devel library to compile libkerosin])
		else
			rcss_freetype_cppflags=`$FREETYPE --cflags`
			rcss_freetype_libadd=`$FREETYPE --libs`
			AC_MSG_CHECKING([freetype include flags])
			AC_MSG_RESULT([$rcss_freetype_cppflags])
			AC_SUBST(FREETYPE_CPPFLAGS, [$rcss_freetype_cppflags])
			AC_MSG_CHECKING([freetype libs file])
			AC_MSG_RESULT([$rcss_freetype_libadd])
			AC_SUBST(FREETYPE_LDFLAGS, [$rcss_freetype_libadd])
		fi
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
       AM_CONDITIONAL(NO_RPATH, true)
       AC_MSG_RESULT([yes])
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
		AC_MSG_CHECKING([if linking against libGL succeeds])
		rcss_tmp="$LDFLAGS"
		LDFLAGS="$LDFLAGS $rcss_GL_LIBADD $rcss_GL_LDFLAGS"
 		AC_LINK_IFELSE([#include <OpenGL/gl.h>
				int main(int argc, char **argv) { glColor3f(0,0,0); }],
				[AC_MSG_RESULT([yes])],
				[AC_MSG_RESULT([no])
				 AC_MSG_ERROR([to build libkerosin, set LDFLAGS so that libGL can be found])])
		LDFLAGS="$rcss_tmp"
		if test "$have_glut" = yes; then
			AC_MSG_CHECKING([if linking against libglut succeeds])
			LDFLAGS="$LDFLAGS $rcss_GL_LIBADD $rcss_GL_LDFLAGS"
	 		AC_LINK_IFELSE([#include <GLUT/glut.h>
					int main(int argc, char **argv) { glutMainLoop(); }],
					[AC_MSG_RESULT([yes])],
					[AC_MSG_RESULT([no])
					 AC_MSG_ERROR([to build libkerosin, set LDFLAGS so that libglut or can be found])
					 ])
			LDFLAGS="$rcss_tmp"
		fi
    else
       AC_MSG_RESULT([sorry])
	# check for OpenGL location and used extensions
    		AC_CHECK_HEADERS([GL/gl.h GL/glu.h],,
                                AC_MSG_ERROR([not all required OpenGL headers could not be found. Please specify the location  of the OpenGL header directory using the CPPFLAGS environment variable]))
    		AC_CHECK_HEADERS([GL/glut.h],[have_glut=yes],[have_glut=no])
    		AM_CONDITIONAL(BUILD_GLUT, test x$have_glut = xyes)
			AC_CHECK_HEADERS([GL/glx.h], AC_SUBST([GLTARGET], [x]),
                        		 AC_CHECK_HEADERS([GL/wglext.h],
                                   	 AC_SUBST([GLTARGET], [win32]),
                                         AC_MSG_ERROR([could not find
 X nor MS Windows GL extensions]),
 				          [#include <GL/gl.h>]),
                          [#include <GL/gl.h>])

	# subst'ing the directory where the prepocessor finds gl.h
	# JAN: the awk command may have to be state more precisely, if problems with distris occur
        GLDIR=`echo "#include <GL/gl.h>" | cpp -M | awk '/gl.h/ { print @S|@2 }'`
        GLDIR=`dirname "$GLDIR"`
        AC_SUBST([GLDIR], [$GLDIR])

	# checking if linking against libGL succeeds
		AC_MSG_CHECKING([if linking against libGL succeeds])
		rcss_GL_LIBADD="-lGL"
		rcss_GL_LDFLAGS=""
		rcss_tmp="$LDFLAGS"
		LDFLAGS="$LDFLAGS $rcss_GL_LIBADD $rcss_GL_LDFLAGS"
 		AC_LINK_IFELSE([#include <GL/gl.h>
				int main(int argc, char **argv) { glColor3f(0,0,0); }],
				[AC_MSG_RESULT([yes])],
				[AC_MSG_RESULT([no])
				 AC_MSG_ERROR([to build libkerosin, set LDFLAGS so that libGL can be found])])
		LDFLAGS="$rcss_tmp"

		AC_MSG_CHECKING([if linking against libGLU succeeds])
		rcss_GL_LIBADD="-lGL -lGLU"
		rcss_GL_LDFLAGS=""
		LDFLAGS="$LDFLAGS $rcss_GL_LIBADD $rcss_GL_LDFLAGS"
 		AC_LINK_IFELSE([#include <GL/glu.h>
				int main(int argc, char **argv) { gluPickMatrix(0,0,0,0,0); }],
				[AC_MSG_RESULT([yes])],
				[AC_MSG_RESULT([no])
				 AC_MSG_ERROR([to build libkerosin, set LDFLAGS so that libGLU can be found])])
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
					 AC_MSG_ERROR([to build libkerosin, set LDFLAGS so that libglut or can be found])
					 ])
			LDFLAGS="$rcss_tmp"
		fi
  fi
  AC_SUBST([GL_LIBADD], [$rcss_GL_LIBADD])
  AC_SUBST([GL_LDFLAGS], [$rcss_GL_LDFLAGS])
]) # RCSS_CHECK_GL


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
                              AC_CHECK_HEADER([SDL.h],,
                              AC_MSG_ERROR([Simple DirectMedia Layer (SDL) headers not found.
Please set CPPFLAGS appropriately or you can specify the location of the SDL installation using the SDL environment variable (e.g. ./configure SDL=$HOME/SDL)]))
                              AC_LINK_IFELSE([#include <SDL.h>
                                              int main(int argc, char **argv) { return SDL_Init(0); }],,
        		      AC_MSG_ERROR([The SDL library (libSDL.a or libSDL.so) cannot be found.
Please set LDFLAGS appropriately or you can specify the location of the SDL installation using the SDL environment variable (e.g. ./configure SDL=$HOME/SDL)]))
	CPPFLAGS=$rcss_tmp_CPPFLAGS
	LDFLAGS=$rcss_tmp_LDFLAGS
        AC_SUBST([SDL_LIBADD], [$rcss_SDL_LIBADD])
        AC_SUBST([SDL_LDFLAGS], [$rcss_SDL_LDFLAGS])
        AC_SUBST([SDL_CPPFLAGS], [$rcss_SDL_CPPFLAGS])
]) # RCSS_CHECK_SDL

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
