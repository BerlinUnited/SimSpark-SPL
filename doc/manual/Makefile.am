if COND_PDFLATEX
%.toc: %.tex
	touch $@
	pdflatex $<

%.pdf: %.tex %.toc 
	pdflatex $<

all-local:
	@if test -f manual.tex; then \
	  $(MAKE) $(AM_MAKEFLAGS) manual.pdf ; \
	else \
	  echo "***" ;\
	  echo "*** The sources for the developers manual are in the CVS version only." ; \
	  echo "***" ; \
	fi

EXTRA_DIST = manual.pdf
CLEANFILES = manual.log manual.pdf manual.aux manual.toc manual.bib manual.dvi manual.out
endif