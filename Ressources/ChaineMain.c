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
    /*
    FILE *f2 = fopen("00014_burma_bis.cha", "w");
    ecrireChaines(test, f2);
    fclose(f2);
    
    //test fonction afficheChainesSVG()
    afficheChainesSVG(test, "00014_burma");

    printf("longueur totale = %lf\n", longueurTotale(test));
*/
    return 0;
}