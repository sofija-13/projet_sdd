#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "Chaine.h"
#include "SVGwriter.h"

#define LIGNE 256

Chaines* lectureChaines(FILE *f){
    if (f == NULL){ // test arguments
        printf("Erreur lectureChaines : FILE NULL\n");
        return NULL;
    }
    // nouvelle chaine
    Chaines *nv = (Chaines*)malloc(sizeof(Chaines));
    fscanf(f, "NbChain: %d\n", &(nv->nbChaines));
    fscanf(f, "Gamma: %d\n", &(nv->gamma));
    nv->chaines = NULL;  

    // pour chaque cellChaine de la Chaine
    for (int i = 0; i < nv->nbChaines; i++){
        CellChaine *c = (CellChaine*)malloc(sizeof(CellChaine));
        fscanf(f, "%d", &(c->numero));
        c->points = NULL;
        int nbPoints;
        fscanf(f, "%d", &nbPoints);

        //pour chaque point de la chaine
        for (int j = 0; j < nbPoints; j++) {
            CellPoint *p = (CellPoint*)malloc(sizeof(CellPoint));
            // lit les coordonnées x, y du point depuis le fichier
            fscanf(f, "%lf %lf", &(p->x), &(p->y));
             // ajoute le point en tete de la liste des points de la chaine
            p->suiv = c->points;
            c->points = p;
        }
        // ajout en tete de la liste des chaines
        c->suiv = nv->chaines;
        nv->chaines = c;
    }
    return nv;
}

void ecrireChaines(Chaines *C, FILE *f){
    if (f==NULL || C==NULL){ // test arguments
        printf("ecrireChaines : argument NULL\n");
        return;
    }
    //ecrit le nombre de chaînes et gamma dans le fichier
    fprintf(f, "NbChain: %d\nGamma: %d\n", C->nbChaines, C->gamma);
    fprintf(f, "\n");
    CellChaine *temp = C->chaines;
    //pour chaque cellChaine de la Chaine
    while (temp){
        int nb_points=0;
        // ecrit le numéro de la chaîne dans le fichier
        fprintf(f, "%d ", temp->numero);
        CellPoint* temp_points=temp->points;
        while(temp_points){ // compte nombre de points dans la chaine
            nb_points++;
            temp_points = temp_points->suiv;
        }
        fprintf(f, "%d ", nb_points);
        // ecrit les coordonnées x, y de chaque point dans la chaîne
        CellPoint* temp_points2=temp->points;
        while(temp_points2){
            fprintf(f, "%.2lf %.2lf ", temp_points2->x, temp_points2->y);
            temp_points2 = temp_points2->suiv;
        }
        fprintf(f, "\n");
        temp = temp->suiv;
    }
} 

// fonction fournie
void afficheChainesSVG(Chaines *C, char* nomInstance){
    if (C==NULL){
        printf("afficheChainesSVG : argument NULL\n");
        return;
    }
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

double longueurChaine(CellChaine *C){
    if (C == NULL){ 
        printf("longueurTotale : C vide\n");
        return 0.0; //retourne 0.0 si C est vide (NULL)
    }
    double longueur = 0.0;
    CellPoint *A = C->points;
    CellPoint *B = C->points->suiv;
    while(B){
        longueur += sqrt((B->x - A->x)*(B->x - A->x) + (B->y - A->y)*(B->y - A->y));
        A = A->suiv;
        B = B->suiv;
    }
    return longueur;
}

double longueurTotale(Chaines *C){
    if (C == NULL){
        printf("longueurTotale : c vide\n");
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
    if (C==NULL || C->chaines==NULL){ 
        printf("comptePointsTotal : C vide\n");
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
    //printf("Nombre total de points = %d\n", nbPoint);
    return nbPoint;
}

void liberer_chaines(Chaines *C){
    if (C==NULL){
        printf("liberer_chaines : C deja vide\n");
        return;
    }
    while(C->chaines){
        CellChaine* tempC = C->chaines;
        C->chaines = C->chaines->suiv;
        
        while(tempC->points){
            CellPoint* tempP = tempC->points;
            tempC->points = tempC->points->suiv;
            free(tempP);
        }
        free(tempC->points);
        free(tempC);
    }
    free(C->chaines);
    free(C);
}
