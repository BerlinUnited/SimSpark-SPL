pkglib_LTLIBRARIES = libclassserver.la
libclassserver_la_SOURCES = classserver.cc classserver.h

# This flag accepts an argument of the form
# `CURRENT[:REVISION[:AGE]]'.  So, passing `-version-info 3:12:1' sets
# CURRENT to 3, REVISION to 12, and AGE to 1. (see below for details)
libclassserver_la_LDFLAGS = -version-info 0:0:0


# If either REVISION or AGE are omitted, they default to 0.  Also note
# that AGE must be less than or equal to the CURRENT interface number.

# Here are a set of rules to help you update your library version
# information:

#  1. Start with version information of `0:0:0' for each libtool library.

#  2. Update the version information only immediately before a public
#     release of your software.  More frequent updates are unnecessary,
#     and only guarantee that the current interface number gets larger
#     faster.

#  3. If the library source code has changed at all since the last
#     update, then increment REVISION (`C:R:A' becomes `C:r+1:A').

#  4. If any interfaces have been added, removed, or changed since the
#     last update, increment CURRENT, and set REVISION to 0.

#  5. If any interfaces have been added since the last public release,
#     then increment AGE.

#  6. If any interfaces have been removed since the last public release,
#     then set AGE to 0.
