#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "Reseau.h"
#include "SVGwriter.h"

Noeud* rechercheCreeNoeudListe(Reseau *R, double x, double y){
    if (R==NULL){ // test validite arguments
        printf("Erreur rechercheCreeNoeudListe : R vide\n");
        return NULL;
    }

    CellNoeud *temp = R->noeuds;
    //recherche noeud de coordonnees (x,y)
    while(temp){
        if (temp->nd->x==x && temp->nd->y==y){
            return temp->nd;
        }
        temp = temp->suiv;
    }
    
    //creation nouveau CellNoeud car absent
    CellNoeud *nv = (CellNoeud*) malloc(sizeof(CellNoeud));
    nv->nd = (Noeud*)malloc(sizeof(Noeud));
    nv->nd->num = (R->nbNoeuds)+1;
    nv->nd->x = x;
    nv->nd->y = y;
    nv->nd->voisins = NULL;

    //ajout en tete de R->noeuds
    nv->suiv = R->noeuds;
    R->noeuds = nv;
    R->nbNoeuds++;

    return nv->nd;
}

int rechercheVoisin(Noeud *nd1, Noeud *nd2){
    // test validite des arguments
    if (nd1 == NULL || nd2 == NULL){
        printf("Erreur rechercheVoisin : argument vide\n");
        return -1;
    }

    // parcours de la liste des voisins de nd1 pour voir si nd2 est deja voisin
    CellNoeud *temp = nd1->voisins;
    while(temp){
        if (temp->nd == nd2) {
            return 1; // nd2 est voisin de nd1
        }
        temp = temp->suiv;
    }

    return 0; // nd2 n'est pas voisin de nd1
}

void ajouterVoisin(Noeud* noeud, Noeud* voisin) {
    // test validite des arguments
    if (noeud == NULL || voisin == NULL) {
        printf("Erreur ajouterVoisin : argument vide\n");
        return;
    }

    // malloc pour la nouvelle cellule de voisin
    CellNoeud* temp = (CellNoeud*)malloc(sizeof(CellNoeud));
    temp->nd = voisin;
    temp->suiv = noeud->voisins;

    // MAJ de la liste des voisins du noeud
    noeud->voisins = temp;
}

Reseau* reconstitueReseauListe(Chaines *C){
    if (C == NULL || C->nbChaines == 0) { // test validite des arguments
        printf("Erreur reconstitueReseauListe : C == NULL ou 0 chaine\n");
        return NULL;
    }
    // nouveau reseau
    Reseau* res = (Reseau*)malloc(sizeof(Reseau));
    res->nbNoeuds = 0;
    res->gamma = C->gamma;
    res->noeuds = NULL;
    res->commodites = NULL;
    
    // "on parcourt une a une chaque chaine"
    CellChaine *tempC = C->chaines;
    while(tempC){
        CellPoint *tempP = tempC->points;

        // premier noeud de la chaine
        Noeud* n1 = rechercheCreeNoeudListe(res, tempP->x, tempP->y);

        // "pour chaque point de la chaine"
        while (tempP->suiv){
            // noeud suivant
            Noeud* n2 = rechercheCreeNoeudListe(res, tempP->suiv->x, tempP->suiv->y);
            
            if (rechercheVoisin(n1, n2) == 0){ // si n2 n'est pas deja dans la liste des voisins de n1
                ajouterVoisin(n1, n2);
                ajouterVoisin(n2, n1);
            }

            n1 = n2;
            tempP = tempP->suiv;
        }

        // "on conserve la commodite de la chaine"        
        CellCommodite* tempCom = (CellCommodite*)malloc(sizeof(CellCommodite));
        if (tempCom != NULL) {
            tempCom->extrA = n1;
            tempCom->extrB = rechercheCreeNoeudListe(res, tempC->points->x, tempC->points->y);
            tempCom->suiv = res->commodites;
            res->commodites = tempCom;
        } 
        tempC = tempC->suiv;
    }

    return res;
}

int nbVoisins(Noeud *nd) {
    if (nd == NULL){ // test validite des arguments
        printf("Erreur nbVoisins : Noeud vide\n");
        return 0;
    }

    int cpt = 0;
    CellNoeud *temp = nd->voisins;

    while (temp){
        cpt++;
        temp = temp->suiv;
    }
    return cpt;
}

int nbLiaisons(Reseau *R) {
    if (R == NULL){ // test validite des arguments
        printf("Erreur nbLiaisons : Reseau vide\n");
        return -1;
    }
    
    int cpt = 0;
    CellNoeud *temp = R->noeuds;
    
    while (temp) {
        cpt += nbVoisins(temp->nd);
        temp = temp->suiv;
    }
    return cpt / 2; // car liaisons comptées 2 fois
}

int nbCommodites(Reseau *R){
    if (R == NULL){ // test validite des arguments
        printf("Erreur nbCommodites : reseau NULL\n");
        return 0;
    }
    CellCommodite* temp = R->commodites;
    int res=0;
    while(temp){
        res++;
        temp=temp->suiv;
    }
    return res;
}

void ecrireReseau(Reseau *R, FILE *f) {
    // ecriture premieres lignes du fichier
    fprintf(f, "NbNoeuds: %d\n", R->nbNoeuds);
    fprintf(f, "NbLiaisons: %d\n", nbLiaisons(R));
    fprintf(f, "NbCommodites: %d\n", nbCommodites(R));
    fprintf(f, "Gamma: %d\n\n", R->gamma);

    // coordonnees et numero des noeuds
    CellNoeud *tempN = R->noeuds;
    while (tempN) {
        fprintf(f, "v %d %lf %lf\n", tempN->nd->num, tempN->nd->x, tempN->nd->y);
        tempN = tempN->suiv;
    }
    fprintf(f, "\n");

    // liaisons 
    tempN = R->noeuds;
    while (tempN) {
        CellNoeud *tempN2 = tempN->nd->voisins;
        while (tempN2) {
            if ( tempN2->nd->num < tempN->nd->num) {
                fprintf(f, "l %d %d\n", tempN2->nd->num,tempN->nd->num );
            }
            tempN2 = tempN2->suiv;
        }
        tempN = tempN->suiv;
    }
    fprintf(f, "\n");

    // commodites
    CellCommodite *tempC = R->commodites;
    while (tempC) {
        fprintf(f, "k %d %d\n", tempC->extrB->num, tempC->extrA->num);
        tempC = tempC->suiv;
    }
}

// fonction donnée
void afficheReseauSVG(Reseau *R, char* nomInstance){
    CellNoeud *courN,*courv;
    SVGwriter svg;
    double maxx=0,maxy=0,minx=1e6,miny=1e6;

    courN=R->noeuds;
    while (courN!=NULL){
        if (maxx<courN->nd->x) maxx=courN->nd->x;
        if (maxy<courN->nd->y) maxy=courN->nd->y;
        if (minx>courN->nd->x) minx=courN->nd->x;
        if (miny>courN->nd->y) miny=courN->nd->y;
        courN=courN->suiv;
    }
    SVGinit(&svg,nomInstance,500,500);
    courN=R->noeuds;
    while (courN!=NULL){
        SVGpoint(&svg,500*(courN->nd->x-minx)/(maxx-minx),500*(courN->nd->y-miny)/(maxy-miny));
        courv=courN->nd->voisins;
        while (courv!=NULL){
            if (courv->nd->num<courN->nd->num)
                SVGline(&svg,500*(courv->nd->x-minx)/(maxx-minx),500*(courv->nd->y-miny)/(maxy-miny),500*(courN->nd->x-minx)/(maxx-minx),500*(courN->nd->y-miny)/(maxy-miny));
            courv=courv->suiv;
        }
        courN=courN->suiv;
    }
    SVGfinalize(&svg);
}

void liberer_noeuds(CellNoeud *noeuds) {
    // liberation noeuds et leurs voisins
    while (noeuds) {
        CellNoeud *temp = noeuds->suiv;

        // liberation voisins du noeud
        CellNoeud *temp2 = noeuds->nd->voisins;
        while (temp2) {
            CellNoeud *temp3 = temp2->suiv;
            free(temp2);
            temp2 = temp3;
        }

        // liberation noeud 
        free(noeuds->nd);
        free(noeuds);
        noeuds = temp;
    }
}

void liberer_reseau(Reseau *R){ 
    if (R == NULL){
        printf("Erreur liberer_reseau : R deja vide\n");
        return; 
    }

    // liberation noeuds et leurs voisins
    liberer_noeuds(R->noeuds);

    // liberation commodites
    CellCommodite *tempC = R->commodites;
    while (tempC) {
        CellCommodite *tempC2 = tempC->suiv;
        free(tempC);
        tempC = tempC2;
    }

    free(R);
}
