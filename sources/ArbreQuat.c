#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "ArbreQuat.h"
#include "SVGwriter.h"

void chaineCoordMinMax(Chaines* C, double* xmin, double* ymin, double* xmax, double* ymax) {
    if (C == NULL) {// si c vide
        printf("Erreur chaineCoordMinMax : Chaine Vide \n");
        return;// return direct
    }

    CellChaine* temp = C->chaines;
    CellPoint* premier = C->chaines->points;
    // On initialise xmin,ymin,xmax et ymax avec les coordonnees du premier point
    *xmin = premier->x;
    *ymin = premier->y;
    *xmax = premier->x;
    *ymax = premier->y;
    while (temp) { // On parcourt les chaînes
        CellPoint* p = temp->points;
        while (p) { // On parcourt les points de la chaine
            if (p->x < *xmin){ // Si p->x < xmin alors xmin = p->x
                *xmin = p->x;
            } 
            if (p->y < *ymin){// Si p->y < ymin alors ymin = p->y;
                *ymin = p->y;
            } 
            if (p->x > *xmax){// Si p->x > *xmax alors *xmax = p->x;
                *xmax = p->x;
            } 
            if (p->y > *ymax){// Si p->y > *ymax alors ymax = p->y;
                *ymax = p->y;
            } 
            p = p->suiv; // On passe au point suivant
        }
        temp = temp->suiv; // On passe a la chaine suivante
    }
}

ArbreQuat* creerArbreQuat(double xc, double yc, double coteX, double coteY) {
    ArbreQuat* aq = (ArbreQuat*)malloc(sizeof(ArbreQuat));
    aq->xc = xc;
    aq->yc = yc;
    aq->coteX = coteX;
    aq->coteY = coteY;
    aq->noeud = NULL;
    aq->so = NULL;
    aq->se = NULL;
    aq->no = NULL;
    aq->ne = NULL;
    return aq;
}

void insererNoeudArbre(Noeud* n, ArbreQuat** a, ArbreQuat* parent) {
    // Si l'arbre est vide 
    if (*a == NULL) {
        //cree un nouvel arbre a l'aide des donnees de l'arbre parent
        double xc, yc, coteX, coteY;
        
        xc = parent->xc;
        yc = parent->yc;
        coteX = parent->coteX / 2;
        coteY = parent->coteY / 2;
        // determine le centre de l'arbre enfant
        if (n->x < xc && n->y < yc) {
            xc -= coteX / 2;
            yc -= coteY / 2;
        } else if (n->x >= xc && n->y < yc) {
            xc += coteX / 2;
            yc -= coteY / 2;
        } else if (n->x < xc && n->y >= yc) {
            xc -= coteX / 2;
            yc += coteY / 2;
        } else {
            xc += coteX / 2;
            yc += coteY / 2;
        }
        //creation de l'arbre
        *a = creerArbreQuat(xc, yc, coteX, coteY);
    }

    // Si l'arbre a deja un noeud,
    //on insere recursivement le noeud actuel et le noeud passe en parametre
    if ((*a)->noeud != NULL) {
        insererNoeudArbre((*a)->noeud, a, *a);
        insererNoeudArbre(n, a, parent);
        (*a)->noeud = NULL;
    } else {
        // Sinon, on recherche le sous-arbre approprie pour inserer le noeud
        ArbreQuat* sous_arbre = NULL;
        if (n->x < (*a)->xc && n->y < (*a)->yc) {
            sous_arbre = (*a)->so;
        } else if (n->x >= (*a)->xc && n->y < (*a)->yc) {
            sous_arbre = (*a)->se;
        } else if (n->x < (*a)->xc && n->y >= (*a)->yc) {
            sous_arbre = (*a)->no;
        } else {
            sous_arbre = (*a)->ne;
        }
        // Insere le noeud dans le sous-arbre trouve
        insererNoeudArbre(n, &sous_arbre, *a);
    }
}

Noeud* rechercheCreeNoeudArbre(Reseau* R, ArbreQuat** a, ArbreQuat* parent, double x, double y) {
    // si l'arbre est vide 
    if (*a == NULL) {
        //cree un nouvel arbre a l'aide des donnees de l'arbre parent
        *a = creerArbreQuat(x, y, parent->coteX / 2, parent->coteX / 2); 
        // recherche ou cree le noeud correspondant dans la liste.
        return rechercheCreeNoeudListe(R, x, y);
    }

    // si l'arbre a deja un noeud avec les memes coordonnees 
    if ((*a)->noeud != NULL) {
        if ((*a)->noeud->x == x && (*a)->noeud->y == y) {
            //on le renvoie
            return (*a)->noeud;
        }
        // sinon, on recherche ou cree le noeud et on l'insere 
        Noeud* n = rechercheCreeNoeudListe(R, x, y);
        insererNoeudArbre(n, a, parent);
        return n;
    }

    // sinon, on recherche le sous-arbre approprie pour continuer la recherche.
    if (x < (*a)->xc && y < (*a)->yc) {
        return rechercheCreeNoeudArbre(R, &((*a)->so), *a, x, y);
    } else if (x >= (*a)->xc && y < (*a)->yc) {
        return rechercheCreeNoeudArbre(R, &((*a)->se), *a, x, y);
    } else if (x < (*a)->xc && y >= (*a)->yc) {
        return rechercheCreeNoeudArbre(R, &((*a)->no), *a, x, y);
    } else {
        return rechercheCreeNoeudArbre(R, &((*a)->ne), *a, x, y);
    }
}


Reseau* reconstitueReseauArbre(Chaines *C){
    if (C == NULL || C->nbChaines == 0) { // si arbre vide 
        printf("Erreur reconstitueReseauArbre : C == NULL ou 0 chaine\n");
        return NULL;//return direct
    }
    // nouveau reseau
    Reseau* res = (Reseau*)malloc(sizeof(Reseau));
    res->nbNoeuds = 0;
    res->gamma = C->gamma;
    res->noeuds = NULL;
    res->commodites = NULL;

    // nouvel arbre quaternaire
    double xmin;
    double ymin; 
    double xmax;
    double ymax;
    chaineCoordMinMax(C, &xmin, &ymin, &xmax, &ymax);
    double coteX = xmax -xmin;
    double coteY = ymax - ymin;
    double xc = coteX/2;
    double yc = coteY/2;
    ArbreQuat* aq = creerArbreQuat(xc,yc,coteX,coteY);
    
    // "on parcourt une a une chaque chaine"
    CellChaine *tempC = C->chaines;
    while(tempC){
        CellPoint *tempP = tempC->points;
        // premier noeud de la chaine
        Noeud* n1 = rechercheCreeNoeudArbre(res, &aq, aq, tempP->x, tempP->y);
        // pour chaque point de la chaine
        while (tempP->suiv){
            // noeud suivant
            Noeud* n2 = rechercheCreeNoeudArbre(res, &aq, aq, tempP->suiv->x, tempP->suiv->y);
            if (rechercheVoisin(n1, n2) == 0){ // si n2 n'est pas deja dans la liste des voisins de n1
                ajouterVoisin(n1, n2);
                ajouterVoisin(n2, n1);
            }
            n1 = n2;
            tempP = tempP->suiv;
        }
        // on conserve la commodite de la chaine     
        CellCommodite* tempCom = (CellCommodite*)malloc(sizeof(CellCommodite));
        if (tempCom != NULL) {
            tempCom->extrA = n1;
            tempCom->extrB = rechercheCreeNoeudArbre(res, &aq, aq, tempC->points->x, tempC->points->y);
            tempCom->suiv = res->commodites;
            res->commodites = tempCom;
        } 
        tempC = tempC->suiv;
    }
    // on libere la memoire de l'arbre qu'on a cree en haut
    libererArbreQuaternaire(aq);
    return res;
}

void libererArbreQuaternaire(ArbreQuat* a) {
    if (a == NULL) {//si arbre vide
        return;//return 
    }
    // on libère récursivement la mémoire pour chaque sous-arbre
    libererArbreQuaternaire(a->so);
    libererArbreQuaternaire(a->se);
    libererArbreQuaternaire(a->no);
    libererArbreQuaternaire(a->ne);

    // si le noeude de l'arbre est n'est pas vide
    if (a->noeud != NULL) {
        //on libère la memoire des noeuds voisin
        CellNoeud* temp = a->noeud->voisins;
        while (temp != NULL) {
            CellNoeud* temp2 = temp;
            temp = temp->suiv;
            free(temp2);
        }
        //du noeud 
        free(a->noeud);
    }
    //puis de l'arbre
    free(a);
}
