if COND_PDFLATEX
%.toc: %.tex
	touch $@
	pdflatex $<

%.pdf: %.tex %.toc 
	pdflatex $<

all-local:
	@if test -f user-manual.tex; then \
	  $(MAKE) $(AM_MAKEFLAGS) user-manual.pdf ; \
	else \
	  echo "***" ;\
	  echo "*** The user's manual (PDF) can be found in doc/users." ; \
	  echo "***" ; \
	fi

EXTRA_DIST = user-manual.pdf
CLEANFILES = user-manual.log user-manual.pdf user-manual.aux user-manual.toc
endif