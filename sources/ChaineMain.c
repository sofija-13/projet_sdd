#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Chaine.h"

int main(){
    srand(time(NULL));
    //lecture dans fichier
    FILE *f1 = fopen("00014_burma.cha", "r");
    Chaines* test = lectureChaines(f1);
    fclose(f1);
    
    //reecriture dans fichier
    FILE *f2 = fopen("test_ecriture.cha", "w");
    ecrireChaines(test, f2);
    fclose(f2);

    //test fonction afficheChainesSVG()
    afficheChainesSVG(test, "test_rpz");
    // printf("longueur totale = %lf\n", longueurTotale(test));

    liberer_chaines(test);
    return 0;
}