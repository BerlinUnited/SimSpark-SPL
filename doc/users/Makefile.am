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
	  echo "*** There is no users manual yet. Please consider contributing to it." ; \
	  echo "***" ; \
	fi

CLEANFILES = manual.log manual.pdf manual.aux manual.toc
endif