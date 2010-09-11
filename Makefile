OUTDIR = bin

unblorb: init unblorb.c
	gcc -Wall unblorb.c -o $(OUTDIR)/unblorb

init:
	mkdir -p bin

clean:
	rm $(OUTDIR)/*
