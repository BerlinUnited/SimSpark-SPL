# RCSS_PATH_RUBY
# Substitutes: @RUBY@		ruby command
#              @RUBY_CPPFLAGS@  additional CPPFLAGS to use to get ruby.h
#	       @RUBY_LDFLAGS@   additional LDFLAGS to use when linking libruby
#	       @RUBY_LDADD@     additional libraries libruby depends on  
#
# original version from the Swig list (major changes after copying)
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
	RUBY_LDFLAGS="-L$ruby_libdir"

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
		    [ruby="$withval"],
		    [ruby="freetype-config"]
	)
        AC_PATH_PROGS(FREETYPE,[$freetype freetype-config],no)
	if test $FREETYPE = "no"; then
		AC_MSG_ERROR([need freetype2 devel library to compile])
	fi

	rcss_freetype_cppflags=`$FREETYPE --cflags`
	rcss_freetype_libadd=`$FREETYPE --libtool`
	AC_MSG_CHECKING([freetype include flags])
	AC_MSG_RESULT([$rcss_freetype_cppflags])
	AC_SUBST(FREETYPE_CPPFLAGS, [$rcss_freetype_cppflags])
	AC_MSG_CHECKING([freetype libtool file])
	AC_MSG_RESULT([$rcss_freetype_libadd])
	AC_SUBST(FREETYPE_LIBADD, [$rcss_freetype_libadd])

]) # RCSS_PATH_FREETYPE
