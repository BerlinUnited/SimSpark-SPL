all-local:
if DOXYGEN
	@echo "***"
	@echo "*** To build the Doxygen API documentation, use 'make doc'"
	@echo "***"
endif

doc: Doxyfile
	rm -rf $(srcdir)/html/developers
	mkdir --parents $(srcdir)/html/developers
	cp *.html html/developers
	doxygen Doxyfile
if DOXYGEN
	doxygen Doxyfile
else
	touch html/index.html
endif

install-data-local: doc
	mkdir --parents $(DESTDIR)$(pkgdatadir)/html/developers
	$(INSTALL_DATA) $(srcdir)/html/*.* $(DESTDIR)$(pkgdatadir)/html/

maintainer-clean-local:
	rm -rf html
	rm -rf *~

EXTRA_DIST = TEXT_INSTEAD_OF_A_MANUAL.txt \
fileserver.html \
howto_agent.html \
howto_class.html \
howto_control.html \
howto_effector.html \
howto_export.html \
howto_function.html \
howto_perceptor.html \
howto_register.html \
howto_script.html \
howto_simulation.html \
howtos.html \
implementation.html \
index.html \
logserver.html \
menu.html \
scriptserver.html \
welcome.html
