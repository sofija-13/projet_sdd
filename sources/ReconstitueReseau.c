#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Reseau.h"

int main(int argc, char** argv){
    /*if (argc != 3){
        printf("Usage : %s <nom fichier .cha> <methode a utiliser>\n", argv[0]);
        printf("Méthodes :\n");
        printf("0\tliste chainee\n");
        printf("1\ttable de hachage\n");
        printf("2\tarbre\n");
        return 1;
    }*/

    //Reseau *reconstitution = NULL;
    //FILE *f = fopen(argv[1], "r");
    FILE *f = fopen("00014_burma.cha", "r");
    Chaines* c = lectureChaines(f);
    Reseau* R = reconstitueReseauListe(c);
    fclose(f);
    
    FILE *f2 = fopen("test_00014", "w");
    ecrireReseau(R,f2);
    fclose(f2);
    
    /*
    switch(atoi(argv[2])){
    case 0: //liste chainee
        reconstitution = reconstitueReseauListe(c);

        break;
    
    case 1:
        reconstitution = reconstitueReseauHachage(c);

        break;
    case 2:
        reconstitution = reconstitueReseauArbre(c);

        break;
    
    }
    */
    // liberation memoire allouee pour Reseau et Chaines
    liberer_chaines(c);
    liberer_reseau(R);

    // liberer_reseau(reconstitution);

    return 0;
}