all-local: Doxyfile
	doxygen Doxyfile
	rm -rf $(srcdir)/html/developers
	mkdir --parents $(srcdir)/html/developers
	cp *.html html/developers

install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(pkgdatadir)/html
	$(INSTALL_DATA) $(srcdir)/html/* $(DESTDIR)$(pkgdatadir)/html/

maintainer-clean-local:
	rm -rf html
	rm -rf *~
