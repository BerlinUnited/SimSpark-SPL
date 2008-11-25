### utility subdir has to be made first
SUBDIRS = wxutil src res

pkginclude_HEADERS = rsgedit_config.h

CLEANFILES = 
dist_doc_DATA = doc/rsgedit.txt AUTHORS ChangeLog COPYING NEWS README TODO

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

