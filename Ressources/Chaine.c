#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "Chaine.h"
#include "SVGwriter.h"

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
} // A REVOIR -- erreur execution

void afficheChainesSVG(Chaines *C, char* nomInstance){
    // int i;
    double maxx=0, maxy=0, minx=1e6, miny=1e6;
    CellChaine *ccour;
    CellPoint *pcour;
    double precx,precy;
    SVGwriter svg;
    ccour=C->chaines;
    while (ccour!=NULL){
        pcour=ccour->points;
        while (pcour!=NULL){
            if (maxx<pcour->x) maxx=pcour->x;
            if (maxy<pcour->y) maxy=pcour->y;
            if (minx>pcour->x) minx=pcour->x;
            if (miny>pcour->y) miny=pcour->y;  
            pcour=pcour->suiv;
        }
    ccour=ccour->suiv;
    }
    SVGinit(&svg,nomInstance,500,500);
    ccour=C->chaines;
    while (ccour!=NULL){
        pcour=ccour->points;
        SVGlineRandColor(&svg);
        SVGpoint(&svg,500*(pcour->x-minx)/(maxx-minx),500*(pcour->y-miny)/(maxy-miny)); 
        precx=pcour->x;
        precy=pcour->y;  
        pcour=pcour->suiv;
        while (pcour!=NULL){
            SVGline(&svg,500*(precx-minx)/(maxx-minx),500*(precy-miny)/(maxy-miny),500*(pcour->x-minx)/(maxx-minx),500*(pcour->y-miny)/(maxy-miny));
            SVGpoint(&svg,500*(pcour->x-minx)/(maxx-minx),500*(pcour->y-miny)/(maxy-miny));
            precx=pcour->x;
            precy=pcour->y;    
            pcour=pcour->suiv;
        }
        ccour=ccour->suiv;
    }
    SVGfinalize(&svg);
}

double longueurChaine(CellChaine *c){
    if (!c){
        printf("Erreur longueurChaine() : chaine vide\n");
        return 0.0;
    }
    double res = 0.0;
    CellPoint *A = c->points;
    CellPoint *B = c->points->suiv;
    while(B){
        res += sqrt((B->x - A->x)*(B->x - A->x) + (B->y - A->y)*(B->y - A->y));
        A = A->suiv;
        B = B->suiv;
    }
    return res;
}

double longueurTotale(Chaines *C){
    if (!C){
        printf("Erreur longueurTotale() : chaine vide\n");
        return 0.0;
    }
    double total = 0.0;
    CellChaine* temp = C->chaines;
    while(temp){
        total += longueurChaine(temp);
        temp=temp->suiv;
    }
    return total;
}
/*
int comptePointsTotal(Chaines *C);
*/