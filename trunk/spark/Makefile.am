### utility subdir has to be made first
SUBDIRS = utility lib plugin test doc spark data

pkginclude_HEADERS = \
sparkconfig.h

doc:	doc-recursive
doc-recursive:	
	cd doc && $(MAKE) $(AM_MAKEFLAGS) doc

CLEANFILES = 

DISTCLEANFILES = $(top_builddir)/macosX/SDLMain.o $(top_builddir)/macosX/libSDLmain.a
EXTRA_DIST = README macosX  \
            windows/sparkconfig.h windows/simspark.iss windows/spark.ico

EXTRA_SRC_DISTS =
EXTRA_BIN_DISTS =
include $(top_srcdir)/config.aux/dist.am

UPLOAD_BIN =
UPLOAD_SRC = upload-gzip upload-bzip2 upload-zip
UPLOAD_TARGETS = \
{gzip=>@PACKAGE@-@VERSION@.tar.gz} \
{bzip2=>@PACKAGE@-@VERSION@.tar.bz2} \
{zip=>@PACKAGE@-@VERSION@.zip}
include $(top_srcdir)/config.aux/sf.am


include $(top_srcdir)/config.aux/cvs.am

#MSI_SETUP_FILE=Setup/Release/@PACKAGE_NAME@Setup.msi
#include $(top_srcdir)/config.aux/msi.am

#include $(top_srcdir)/config.aux/rpm.am

