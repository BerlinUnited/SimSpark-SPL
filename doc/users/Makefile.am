%.toc: %.tex
	touch $@
	pdflatex $<

%.pdf: %.tex %.toc 
	pdflatex $<

noinst_DATA = manual.pdf

CLEANFILES = manual.log manual.pdf manual.aux manual.toc
