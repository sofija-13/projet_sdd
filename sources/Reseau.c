#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "Reseau.h"
#include "SVGwriter.h"

Noeud* rechercheCreeNoeudListe(Reseau *R, double x, double y){
    if (R==NULL){ // test validite arguments
        printf("Erreur rechercheCreeNoeudListe : reseau NULL\n");
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
    CellNoeud *nouveau = (CellNoeud*) malloc(sizeof(CellNoeud));
    nouveau->nd = (Noeud*)malloc(sizeof(Noeud));
    nouveau->nd->num = (R->nbNoeuds)+1;
    nouveau->nd->x = x;
    nouveau->nd->y = y;
    nouveau->nd->voisins = NULL;

    //ajout en tete de R->noeuds
    nouveau->suiv = R->noeuds;
    R->noeuds = nouveau;
    R->nbNoeuds++;

    return nouveau->nd;
}

int rechercheVoisin(Noeud *nd1, Noeud *nd2){
    // test validite des arguments
    if (nd1 == NULL || nd2 == NULL){
        printf("Erreur rechercheVoisin : argument NULL\n");
        return -1;
    }

    // parcours de la liste des voisins de nd1 pour voir si nd2 est deja voisin
    CellNoeud *voisin = nd1->voisins;
    while(voisin){
        if (voisin->nd == nd2) {
            return 1; // nd2 est voisin de nd1
        }
        voisin = voisin->suiv;
    }

    return 0; // nd2 n'est pas voisin de nd1
}

void ajouterVoisin(Noeud* noeud, Noeud* voisin) {
    // test validite des arguments
    if (noeud == NULL || voisin == NULL) {
        printf("Erreur ajouterVoisin : argument NULL\n");
        return;
    }

    // Allocation de mémoire pour la nouvelle cellule de voisin
    CellNoeud* nouveau_voisin = (CellNoeud*)malloc(sizeof(CellNoeud));
    if (nouveau_voisin == NULL) {
        printf("Erreur d'allocation de mémoire pour la cellule de voisin\n");
        return;
    }

    // Attribution du voisin au nouveau voisin
    nouveau_voisin->nd = voisin;
    nouveau_voisin->suiv = noeud->voisins;

    // Mise à jour de la liste des voisins du noeud
    noeud->voisins = nouveau_voisin;
}

Reseau* reconstitueReseauListe(Chaines *C){
    if (C == NULL || C->nbChaines == 0) { // test validite des arguments
        printf("Erreur reconstitueReseauListe : C == NULL ou 0 chaine\n");
        return NULL;
    }
    // nouveau reseau
    Reseau* res = (Reseau*)malloc(sizeof(Reseau));
    if (res == NULL) {
        printf("Erreur reconstitueReseauListe: erreur malloc pour nouveau reseau\n");
        return NULL;
    }
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
        } else {
            printf("Erreur reconstitueReseauListe : échec malloc commodité\n");
            return NULL;
        }
    
        tempC = tempC->suiv;
    }

    return res;
}

int nbLiaisons(Reseau *R) {
    if (R == NULL) {
        printf("Erreur nbLiaisons : Reseau NULL\n");
        return -1;
    }
    
    int cpt = 0;
    CellNoeud *temp = R->noeuds;
    
    while (temp) {
        cpt += nbVoisins(temp->nd);
        temp = temp->suiv;
    }
    return cpt / 2; // Chaque liaison est comptée deux fois
}

int nbCommodites(Reseau *R){
    CellCommodite* temp = R->commodites;
    int res=0;
    while(temp){
        res++;
        temp=temp->suiv;
    }
    return res;
}

int nbVoisins(Noeud *nd) {
    if (nd == NULL) {
        printf("Erreur nbVoisins : Noeud NULL\n");
        return 0;
    }

    int count = 0;
    CellNoeud *voisin = nd->voisins;
    while (voisin) {
        count++;
        voisin = voisin->suiv;
    }
    return count;
}

int dejaVu(ListeCouple l, int a, int b){
    ListeCouple tmp = l;
    while(l){
        if((tmp->A == a && tmp->B ==b) || (tmp->A == b && tmp->B ==a)){
            return 0;
        }
        tmp = tmp->suiv;
    }
    return 1;
}

void ecrireReseau(Reseau *R, FILE *f) {
    // Écrire les informations générales du réseau dans le fichier
    fprintf(f, "NbNoeuds: %d\n", R->nbNoeuds);
    fprintf(f, "NbLiaisons: %d\n", nbLiaisons(R)); // Utilisez la fonction nbLiaisons pour obtenir le nombre de liaisons
    fprintf(f, "NbCommodites: %d\n", nbCommodites(R)); // Utilisez la fonction nbCommodites pour obtenir le nombre de commodités
    fprintf(f, "Gamma: %d\n\n", R->gamma);

    // Écrire les coordonnées des noeuds dans le fichier
    CellNoeud *temp_noeud = R->noeuds;
    while (temp_noeud) {
        fprintf(f, "v %d %lf %lf\n", temp_noeud->nd->num, temp_noeud->nd->x, temp_noeud->nd->y);
        temp_noeud = temp_noeud->suiv;
    }
    fprintf(f, "\n");

    // Écrire les liaisons dans le fichier
    temp_noeud = R->noeuds;
    while (temp_noeud) {
        CellNoeud *voisin = temp_noeud->nd->voisins;
        while (voisin) {
            if ( voisin->nd->num < temp_noeud->nd->num) {
                fprintf(f, "l %d %d\n", voisin->nd->num,temp_noeud->nd->num );
            }
            voisin = voisin->suiv;
        }
        temp_noeud = temp_noeud->suiv;
    }
    fprintf(f, "\n");

    // Écrire les commodités dans le fichier
    CellCommodite *temp_commodite = R->commodites;
    while (temp_commodite) {
        fprintf(f, "k %d %d\n", temp_commodite->extrB->num, temp_commodite->extrA->num);
        temp_commodite = temp_commodite->suiv;
    }
}

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
    // Libération récursive des noeuds et de leurs voisins
    while (noeuds) {
        CellNoeud *suivant_noeud = noeuds->suiv;

        // Libération des voisins du noeud
        CellNoeud *temp_voisin = noeuds->nd->voisins;
        while (temp_voisin) {
            CellNoeud *suivant_voisin = temp_voisin->suiv;
            free(temp_voisin);
            temp_voisin = suivant_voisin;
        }

        // Libération du noeud lui-même
        free(noeuds->nd);
        free(noeuds);
        noeuds = suivant_noeud;
    }
}

void liberer_reseau(Reseau *R) {
    // Vérification de la validité de l'argument
    if (R == NULL) {
        printf("Erreur liberer_reseau : argument NULL\n");
        return;
    }

    // Libération des noeuds et de leurs voisins
    liberer_noeuds(R->noeuds);

    // Libération des commodités
    CellCommodite *temp_commodite = R->commodites;
    while (temp_commodite) {
        CellCommodite *suivant_commodite = temp_commodite->suiv;
        free(temp_commodite);
        temp_commodite = suivant_commodite;
    }

    // Libération de la structure Reseau
    free(R);
}
