#all

ChaineMain: ChaineMain.o Chaine.o SVGwriter.o
	gcc -Wall -o ChaineMain ChaineMain.o Chaine.o SVGwriter.o -lm

ChaineMain.o: ChaineMain.c 
	gcc -Wall -c ChaineMain.c

Chaine.o: Chaine.c
	gcc -Wall -c Chaine.c

SVGwriter.o: SVGwriter.c
	gcc -Wall -c SVGwriter.c

clean:
	rm *.o ChaineMain test.html test_ecriture.cha