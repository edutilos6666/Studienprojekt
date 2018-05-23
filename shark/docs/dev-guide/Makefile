MAIN = dev-guide
PDF_VIEWER = evince

all: pdf show

pdf: $(MAIN).tex 
	pdflatex -shell-escape $(MAIN).tex
	pdflatex -shell-escape $(MAIN).tex

bib: $(MAIN).tex
	pdflatex -shell-escape $(MAIN).tex
	bibtex $(MAIN).aux
	pdflatex -shell-escape $(MAIN).tex
	pdflatex -shell-escape $(MAIN).tex

clean:
	rm -f $(MAIN).{aux,bbl,blg,log,fls,fdb_latexmk,nav,out,pdf,snm,synctex.gz,toc}

show:
	$(PDF_VIEWER) $(MAIN).pdf &
