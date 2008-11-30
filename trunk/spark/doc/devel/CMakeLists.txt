add_subdirectory(howtos)


########### install files ###############




#original Makefile.am contents follow:

#SUBDIRS = howtos
#
#if COND_PDFLATEX
#%.toc: %.tex
#	touch $@
#	pdflatex $<
#
#%.pdf: %.tex %.toc 
#	pdflatex $<
#
#all-local:
#	@if test -f manual.tex; then \
#	  $(MAKE) $(AM_MAKEFLAGS) manual.pdf ; \
#	else \
#	  echo "***" ;\
#	  echo "*** There is no developers manual yet. Please consider contributing to it." ; \
#	  echo "***" ; \
#	fi
#
#EXTRA_DIST = manual.pdf
#CLEANFILES = manual.log manual.pdf manual.aux manual.toc
#endif
