SUBDIRS = devel

all-local:
if DOXYGEN
	@echo "***"
	@echo "*** To (re-)build the Doxygen API documentation, use 'make doc'"
	@echo "***"
endif

doc: Doxyfile
	rm -rf $(srcdir)/api/developers
	mkdir -p $(srcdir)/api/developers
	cp $(srcdir)/devel/howtos/*.* $(srcdir)/api/developers/
if DOXYGEN
	doxygen Doxyfile
else
	touch $(srcdir)/api/index.html
endif

maintainer-clean-local:
	rm -rf api
	rm -rf *~
