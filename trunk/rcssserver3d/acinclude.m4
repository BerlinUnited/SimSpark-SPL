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

