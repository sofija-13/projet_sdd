#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "ArbreQuat.h"
#include "SVGwriter.h"

void chaineCoordMinMax(Chaines* C, double* xmin, double* ymin, double* xmax, double* ymax){
    if(C == NULL){
        printf("Chaine Vide \n");
        return;
    }

    CellChaine* temp = C->chaines;
    CellPoint* premier = C->chaines->points;
    double Xmin = premier->x;
    double Ymin = premier->y;
    double Xmax = premier->x;
    double Ymax = premier->y;
    while(temp){
        CellPoint* p = temp->points;
        while(p){
            //si coordonnées plus petit que Xmin et Ymin
            if (p->x < Xmin && p->y < Ymin){
                Xmin = p->x;
                Ymin = p->y;
            }
            //si coordonnées plus grand que Xmax et Ymax
            else if (p->x > Xmax && p->y > Ymax){
                Xmax = p->x;
                Ymax = p->y;
            }
            p = p->suiv;
        }
        temp = temp->suiv;
    }

}

ArbreQuat* creerArbreQuat(double xc, double yc, double coteX, double coteY){
    ArbreQuat* aq = (ArbreQuat*)malloc(sizeof(ArbreQuat));

    aq->xc = xc;
    aq->yc = yc;
    aq->coteX = coteX;
    aq->coteY = coteY;

    aq->noeud = NULL;           /* Pointeur vers le noeud du reseau */
    aq->so = NULL;   /* Sous-arbre sud-ouest, pour x < xc et y < yc */
    aq->se = NULL;   /* Sous-arbre sud-est, pour x >= xc et y < yc */
    aq->no = NULL;   /* Sous-arbre nord-ouest, pour x < xc et y >= yc */
    aq->ne = NULL;

    return aq;
}

void insererNoeudArbre(Noeud* n, ArbreQuat** a, ArbreQuat* parent){

    if (*a == NULL){
        double xc, yc, coteX, coteY;
        // en fonction de parent et n, on déterminer les coordonnées du centre de la nouvelle cellule
        if (n->x < parent->xc && n->y < parent->yc) {//sud-ouest
            xc = parent->xc - parent->coteX / 4;
            yc = parent->yc - parent->coteY / 4;
        } else if (n->x >= parent->xc && n->y < parent->yc) {//sud-est
            xc = parent->xc + parent->coteX / 4;
            yc = parent->yc - parent->coteY / 4;
        } else if (n->x < parent->xc && n->y >= parent->yc) {//nord-ouest
            xc = parent->xc - parent->coteX / 4;
            yc = parent->yc + parent->coteY / 4;
        } else {// nord-est
            xc = parent->xc + parent->coteX / 4;
            yc = parent->yc + parent->coteY / 4;
        }
        // Les dimensions sont la moitié des dimensions du parent
        coteX = parent->coteX / 2;
        coteY = parent->coteY / 2;

        creerArbreQuat(xc, yc, coteX, coteY);
        return;
    }

    else if ((*a)->noeud != NULL) { // Cas d'une feuille
        insererNoeudArbre((*a)->noeud, a, parent);
        insererNoeudArbre(n, a, parent);
        (*a)->noeud = NULL;
    } 
    else { // Cas d'une cellule interne
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
        insererNoeudArbre(n, &sous_arbre, *a);
    }
    
}

Noeud* rechercheCreeNoeudArbre(Reseau* R, ArbreQuat** a, ArbreQuat* parent, double x, double y) {
    // Cas où l'arbre est vide
    if (*a == NULL) {
        // Création du nœud correspondant au point
        Noeud* n = rechercheCreeNoeudListe(R, x, y);
        insererNoeudArbre(n, a, parent); // Insérer le nouveau nœud dans l'arbre
        return n; // Retourner le nouveau nœud créé
    }

    // Cas où on est sur une feuille de l'arbre
    if ((*a)->noeud != NULL) {
        // Si le nœud correspond au point
        if ((*a)->noeud->x == x && (*a)->noeud->y == y) {
            return (*a)->noeud;
        }
        // Sinon, créer et insérer le nouveau nœud
        Noeud* n = rechercheCreeNoeudListe(R, x, y);
        insererNoeudArbre(n, a, parent); // Insérer le nouveau nœud dans l'arbre
        return n; // Retourner le nouveau nœud créé
    }

    // Cas où on est sur une cellule interne de l'arbre
    if ((*a)->noeud == NULL) {
        // Déterminer dans quelle cellule chercher le nœud du réseau
        if (x < (*a)->xc && y < (*a)->yc) {
            return rechercheCreeNoeudArbre(R, &((*a)->so), *a, x, y);
        }
        else if (x >= (*a)->xc && y < (*a)->yc) {
            return rechercheCreeNoeudArbre(R, &((*a)->se), *a, x, y);
        }
        else if (x < (*a)->xc && y >= (*a)->yc) {
            return rechercheCreeNoeudArbre(R, &((*a)->no), *a, x, y);
        }
        else if (x >= (*a)->xc && y >= (*a)->yc) {
            return rechercheCreeNoeudArbre(R, &((*a)->ne), *a, x, y);
        }
    }

    return NULL; // Retourne NULL par défaut (cas improbable)
}


Reseau* reconstitueReseauArbre(Chaines *C){
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

        // "pour chaque point de la chaine"
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

        // "on conserve la commodite de la chaine"        
        CellCommodite* tempCom = (CellCommodite*)malloc(sizeof(CellCommodite));
        if (tempCom != NULL) {
            tempCom->extrA = n1;
            tempCom->extrB = rechercheCreeNoeudArbre(res, &aq, aq, tempC->points->x, tempC->points->y);
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



void libererArbreQuaternaire(ArbreQuat* a) {
    if (a == NULL) {
        return;
    }

    // Libérer récursivement les sous-arbres
    libererArbreQuaternaire(a->so);
    libererArbreQuaternaire(a->se);
    libererArbreQuaternaire(a->no);
    libererArbreQuaternaire(a->ne);

    // Libérer la mémoire du nœud (si présent)
    if (a->noeud != NULL) {
        // Libérer la liste des voisins (si présente)
        CellNoeud* current_voisin = a->noeud->voisins;
        while (current_voisin != NULL) {
            CellNoeud* temp = current_voisin;
            current_voisin = current_voisin->suiv;
            free(temp);
        }
        free(a->noeud);
    }

    // Libérer la mémoire de l'arbre
    free(a);
}
