all-local:
if DOXYGEN
	@echo "***"
	@echo "*** To (re-)build the Doxygen API documentation, use 'make apidoc'"
	@echo "***"
endif

install-data-local: $(srcdir)/html/index.html
	mkdir --parents $(DESTDIR)$(pkgdatadir)/html/developers
	$(INSTALL_DATA) $(srcdir)/html/*.* $(DESTDIR)$(pkgdatadir)/html/

apidoc: Doxyfile
	rm -rf $(srcdir)/html/developers
	mkdir --parents $(srcdir)/html/developers
	cp *.html html/developers
if DOXYGEN
	doxygen Doxyfile
else
	touch html/index.html
endif

$(srcdir)/html/index.html: 
	@$(MAKE) apidoc

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
welcome.html \
used_libraries.txt
