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

    fgets(buffer, LIGNES, f);

    //lecture 2 premieres lignes
    sscanf(buffer, "NbChain: %d", &nv->nbChaines);
    fgets(buffer, LIGNES, f);
    sscanf(buffer, "Gamma: %d", &nv->gamma);
    // printf("lu: nbChaines = %d\tGamma = %d\n", nv->nbChaines, nv->gamma);
    
    //lecture reste du fichier
    nv->chaines = NULL;
    fgets(buffer, LIGNES, f);
    for (int i =0; i<nv->nbChaines; i++){
        CellChaine *c = (CellChaine*)malloc(sizeof(CellChaine));
        // printf("%s", buffer);
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

void ecrireChaines(Chaines *C, FILE *f){
    if (!f || !C){ // test validite des arguments
        printf("Erreur ecrireChaines()\n");
        return;
    }
    fprintf(f, "NbChain: %d\nGamma: %d\n", C->nbChaines, C->gamma);
    CellChaine *temp = C->chaines;
    for (int i=0; i<C->nbChaines; i++){

        // printf("i=%d\n", i); fflush(stdout);
        // printf("%d \n", temp->numero); fflush(stdout);
        
        char buffer[LIGNES];
        int nb_points=0;
        char x[30];
        while(temp->points){
            sprintf(x, "%lf %lf ", temp->points->x, temp->points->y);
            strcat(buffer, x);
            // printf("%s\n", buffer);
            nb_points++;
            temp->points = temp->points->suiv;
        }
        fprintf(f, "%d %s\n", nb_points, buffer);
        temp = temp->suiv;
    }
}

/*
void afficheChainesSVG(Chaines *C, char* nomInstance);
double longueurTotale(Chaines *C);
int comptePointsTotal(Chaines *C);
*/