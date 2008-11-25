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


