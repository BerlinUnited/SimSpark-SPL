SUBDIRS = devel users

all-local:
if DOXYGEN
	@echo "***"
	@echo "*** To (re-)build the Doxygen API documentation, use 'make apidoc'"
	@echo "***"
endif

install-data-local: $(srcdir)/api/index.html
	rm -rf $(DESTDIR)$(pkgdatadir)/api/
	rm -rf $(DESTDIR)$(pkgdatadir)/html/
	mkdir -p $(DESTDIR)$(pkgdatadir)/
	cp -r $(srcdir)/api $(DESTDIR)$(pkgdatadir)/
	chmod -R a+rX $(DESTDIR)$(pkgdatadir)/api

apidoc: Doxyfile
	rm -rf $(srcdir)/api/developers
	mkdir -p $(srcdir)/api/developers
	cp $(srcdir)/devel/howtos/*.* $(srcdir)/api/developers/
if DOXYGEN
	doxygen Doxyfile
else
	touch $(srcdir)/api/index.html
endif

$(srcdir)/api/index.html: 
	@$(MAKE) apidoc

maintainer-clean-local:
	rm -rf api
	rm -rf *~

EXTRA_DIST = TEXT_INSTEAD_OF_A_MANUAL.txt
