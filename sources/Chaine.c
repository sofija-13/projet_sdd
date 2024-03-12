#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "Chaine.h"
#include "SVGwriter.h"

#define LIGNE 256

/*
Chaines* lectureChaines(FILE *f){
    if (f==NULL){
        printf("Erreur lectureChaines : FILE f est NULL\n");
        return NULL;
    }
    
    Chaines* nv = (Chaines*)malloc(sizeof(Chaines));
    nv->chaines = NULL;
    
    char buffer[LIGNE];

    //lecture 2 premieres lignes
    fgets(buffer, LIGNE, f);
    sscanf(buffer, "NbChain: %d", &nv->nbChaines);
    
    fgets(buffer, LIGNE, f);
    sscanf(buffer, "Gamma: %d", &nv->gamma);
    
    // printf("nbChaines = %d\tGamma = %d\n", nv->nbChaines, nv->gamma);
    

    //lecture reste du fichier
    fgets(buffer, LIGNE, f); 
    // printf("%s\n", buffer);
    for (int i=0; i<nv->nbChaines; i++){
        // printf("i=%d\n", i);
        CellChaine *c = (CellChaine*)malloc(sizeof(CellChaine));
        int nbPoints = 0;
        // char* reste = (char*)malloc(256*sizeof(char)); //fin de la ligne (coordonnees points)

        sscanf(buffer, "%d %d", &c->numero, &nbPoints);
        // printf("numero=%d nbPoints=%d\n", c->numero, nbPoints);

        char* reste = buffer+3;
        // printf("\treste:\t%s", reste);
        
        for (int j=0; j<nbPoints; j++){
            // printf("\tj=%d\n", j);
            CellPoint *p = (CellPoint*)malloc(sizeof(CellPoint));
            sscanf(reste, "%lf %lf ", &p->x, &p->y);
            // printf("\t %.2lf %.2lf ", p->x, p->y);

            // insertion en tete
            p->suiv = c->points;
            c->points = p;
            reste = reste+12; //MAJ buffer : points suivants
        }
        // printf("\n");
        // insertion en tete
        c->suiv = nv->chaines;
        nv->chaines = c;
        fgets(buffer, LIGNE, f);
        
    }
    return nv;
} ta version provoquait des erreurs de mallocs donc j'en ai réécris une juste en dessous
*/ 
Chaines* lectureChaines(FILE *f) {
    if (f == NULL) {
        printf("Erreur : fichier invalide\n");
        return NULL;
    }

    Chaines *nv = (Chaines*)malloc(sizeof(Chaines));
    if (nv == NULL) {
        printf("Erreur d'allocation de mémoire\n");
        return NULL;
    }

    fscanf(f, "NbChain: %d\n", &(nv->nbChaines));
    fscanf(f, "Gamma: %d\n", &(nv->gamma));

    nv->chaines = NULL;  // Initialisation de la liste des chaines à NULL

    for (int i = 0; i < nv->nbChaines; i++) {
        CellChaine *c = (CellChaine*)malloc(sizeof(CellChaine));
        if (c == NULL) {
            printf("Erreur d'allocation de mémoire\n");
            return NULL;
        }
        fscanf(f, "%d", &(c->numero));
        printf("%d\n",c->numero);
        c->points = NULL;  // Initialisation de la liste des points à NULL

        int nbPoints;
        fscanf(f, "%d", &nbPoints);

        for (int j = 0; j < nbPoints; j++) {
            CellPoint *p = (CellPoint*)malloc(sizeof(CellPoint));
            if (p == NULL) {
                printf("Erreur d'allocation de mémoire\n");
                return NULL;
            }
            fscanf(f, "%lf %lf", &(p->x), &(p->y));
            printf(" %lf %lf \n",p->x, p->y);
            p->suiv = c->points;
            c->points = p;
        }

        c->suiv = nv->chaines;
        nv->chaines = c;
    }

    return nv;
}

void ecrireChaines(Chaines *C, FILE *f){
    if (f==NULL || C==NULL){ // test validite des arguments
        printf("ecrireChaines : argument NULL\n");
        return;
    }
    fprintf(f, "NbChain: %d\nGamma: %d\n", C->nbChaines, C->gamma);
    CellChaine *temp = C->chaines;
    while (temp){
        int nb_points=0;
        fprintf(f, "%d ", temp->numero);
        CellPoint* temp_points=temp->points;
        while(temp_points){ // compte nbre de points
            nb_points++;
            temp_points = temp_points->suiv;
        }
        fprintf(f, "%d ", nb_points);
        CellPoint* temp_points2=temp->points;
        while(temp_points2){
            fprintf(f, "%.2lf %.2lf ", temp_points2->x, temp_points2->y);
            temp_points2 = temp_points2->suiv;
        }
        fprintf(f, "\n");
        temp = temp->suiv;
    }
} 

void afficheChainesSVG(Chaines *C, char* nomInstance){
    if (C==NULL){
        printf("afficheChainesSVG : argument NULL\n");
        return;
    }
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
        printf("longueurTotale : argument NULL\n");
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
        printf("longueurTotale : argument NULL\n");
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

int comptePointsTotal(Chaines *C){
    if (C==NULL || C->chaines==NULL){ // test validite des arguments
        printf("comptePointsTotal : argument NULL\n");
        return 0;
    }
    int nbPoint = 0;
    CellChaine *temp = C->chaines;
    while(temp){
        CellPoint *tempP = temp->points;
        while(tempP){
            nbPoint++;
            tempP = tempP->suiv;
        }
        temp = temp->suiv;
    }
    printf("Nombre total de points = %d\n", nbPoint);
    return nbPoint;
}

void liberer_chaines(Chaines *C){
    if (C==NULL){
        printf("liberer_chaines : argument NULL\n");
        return;
    }
    while(C->chaines){
        CellChaine* temp_chaine = C->chaines;
        C->chaines = C->chaines->suiv;
        
        while(temp_chaine->points){
            CellPoint* temp_point = temp_chaine->points;
            temp_chaine->points = temp_chaine->points->suiv;
            free(temp_point);
        }
        free(temp_chaine->points);
        free(temp_chaine);
    }
    free(C->chaines);
    free(C);
}
