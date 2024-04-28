#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Chaine.h"

int main(){
    srand(time(NULL));

    /*      pour   00014_burma.cha    */

    //lecture dans fichier
    FILE *f1 = fopen("00014_burma.cha", "r");
    Chaines* c1 = lectureChaines(f1);
    fclose(f1);
    //reecriture dans fichier
    FILE *f2 = fopen("test_00014_burma.cha", "w");
    ecrireChaines(c1, f2);
    fclose(f2);
    //test fonction afficheChainesSVG()
    afficheChainesSVG(c1, "test_c1");
    // libere la memoire de la chaine c1
    liberer_chaines(c1);

    /*      pour   05000_USA-road-d-NY.cha    */

    //lecture dans fichier
    FILE *f3 = fopen("05000_USA-road-d-NY.cha", "r");
    Chaines* c2 = lectureChaines(f3);
    fclose(f3);
    //reecriture dans fichier
    FILE *f4 = fopen("test_05000_USA-road-d-NY.cha", "w");
    ecrireChaines(c2, f4);
    fclose(f4);
    //test fonction afficheChainesSVG()
    afficheChainesSVG(c2, "test_c2");
    // libere la memoire de la chaine c2
    liberer_chaines(c2);

    /*      pour   07397_pla.cha    */

    //lecture dans fichier
    FILE *f5 = fopen("07397_pla.cha", "r");
    Chaines* c3 = lectureChaines(f5);
    fclose(f5);
    //reecriture dans fichier
    FILE *f6 = fopen("test_07397_pla.cha", "w");
    ecrireChaines(c3, f6);
    fclose(f6);
    //test fonction afficheChainesSVG()
    afficheChainesSVG(c3, "test_c3");
    // libere la memoire de la chaine c3
    liberer_chaines(c3);

    return 0;
}