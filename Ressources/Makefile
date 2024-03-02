#all

ChaineMain: ChaineMain.o Chaine.o
	gcc -Wall -o ChaineMain ChaineMain.o Chaine.o

ChaineMain.o: ChaineMain.c 
	gcc -Wall -c ChaineMain.c

Chaine.o: Chaine.c
	gcc -Wall -c Chaine.c

clean:
	rm *.o ChaineMain