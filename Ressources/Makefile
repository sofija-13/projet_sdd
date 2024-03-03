#all

ChaineMain: ChaineMain.o Chaine.o SVGwriter.o
	gcc -Wall -lm -o ChaineMain ChaineMain.o Chaine.o SVGwriter.o

ChaineMain.o: ChaineMain.c 
	gcc -Wall -c ChaineMain.c

Chaine.o: Chaine.c
	gcc -Wall -c Chaine.c

SVGwriter.o: SVGwriter.c
	gcc -Wall -c SVGwriter.c

clean:
	rm *.o ChaineMain *.html