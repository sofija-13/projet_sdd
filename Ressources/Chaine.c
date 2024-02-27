#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Chaine.h"

#define LIGNES 256

Chaines* lectureChaines(FILE *f){
    if (!f){
        return NULL;
    }
    
    Chaines* nv = (Chaines*)malloc(sizeof(Chaines));
    char buffer[LIGNES];
    //char *res = 
    fgets(buffer, LIGNES, f);

    //lecture 2 premieres lignes
    sscanf(buffer, "NbChain: %d", &nv->nbChaines);
    fgets(buffer, LIGNES, f);
    sscanf(buffer, "Gamma: %d", &nv->gamma);
    printf("lu: nbChaines = %d\tGamma = %d\n", nv->nbChaines, nv->gamma);
    
    //lecture reste du fichier
    nv->chaines = NULL;
    fgets(buffer, LIGNES, f);
    for (int i =0; i<nv->nbChaines; i++){
        CellChaine *c = (CellChaine*)malloc(sizeof(CellChaine));
        printf("%s", buffer);
        int nbPoints = 0;
        char* reste= malloc(256*sizeof(char)); //fin de la ligne (coordonnees points)
        sscanf(buffer, "%d %d %s", &c->numero, &nbPoints, reste);
        for (int j=0; j<nbPoints; j++){
            char* reste2= malloc(256*sizeof(char));
            CellPoint *p = (CellPoint*)malloc(sizeof(CellPoint));
            sscanf(reste, "%lf %lf %s", &p->x, &p->y, reste2);
            // insertion en tete
            p->suiv = c->points;
            c->points = p;
            reste=reste2;
        }
        
        // insertion en tete
        c->suiv = nv->chaines;
        nv->chaines = c;
        fgets(buffer, LIGNES, f);
        
    }
    return nv;
}

/*
void ecrireChaines(Chaines *C, FILE *f);
void afficheChainesSVG(Chaines *C, char* nomInstance);
double longueurTotale(Chaines *C);
int comptePointsTotal(Chaines *C);
*/