#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Reseau.h"
#include "Hachage.h"
#include "ArbreQuat.h"

int main(int argc, char** argv){
    
    if (argc != 3){
        printf("Usage : %s <nom fichier .cha> <methode a utiliser>\n", argv[0]);
        printf("Méthodes :\n");
        printf("0\tliste chainee\n");
        printf("1\ttable de hachage\n");
        printf("2\tarbre quaternaire\n");
        return 1;
    }

    // lecture chaine dans fichier fourni
    FILE *f = fopen(argv[1], "r");
    Chaines* c = lectureChaines(f);
    fclose(f);
    
    Reseau* R = NULL;

    switch(atoi(argv[2])){
    case 0: //liste chainee
        R = reconstitueReseauListe(c);
        break;
    
    case 1: // table de hachage
        // R = reconstitueReseauHachage(c, 100);
        break;

    case 2: // arbre quaternaire
        // R = reconstitueReseauArbre(c);
        break;
    
    }

    // test fonction de reconstitution
    FILE *f2 = fopen("test_00014.res", "w");
    ecrireReseau(R, f2);
    fclose(f2);
    
    // liberation memoire allouee pour Reseau et Chaines
    liberer_chaines(c);
    liberer_reseau(R);

    return 0;
}