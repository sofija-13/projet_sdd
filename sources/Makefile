all: ChaineMain ReconstitueReseau

ReconstitueReseau: ReconstitueReseau.o Reseau.o Chaine.o SVGwriter.o
	gcc -Wall -o ReconstitueReseau ReconstitueReseau.o Reseau.o Chaine.o SVGwriter.o -lm

ReconstitueReseau.o: ReconstitueReseau.c
	gcc -Wall -c ReconstitueReseau.c
	
Reseau.o: Reseau.c
	gcc -Wall -c Reseau.c

ChaineMain: ChaineMain.o Chaine.o SVGwriter.o
	gcc -Wall -o ChaineMain ChaineMain.o Chaine.o SVGwriter.o -lm

ChaineMain.o: ChaineMain.c 
	gcc -Wall -c ChaineMain.c

Chaine.o: Chaine.c
	gcc -Wall -c Chaine.c

SVGwriter.o: SVGwriter.c
	gcc -Wall -c SVGwriter.c

clean:
	rm *.o *.html ChaineMain ReconstitueReseau