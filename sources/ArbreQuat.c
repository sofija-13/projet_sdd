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

Reseau* reconstitueReseauArbre(Chaines* C){
    if(C == NULL || C->nbChaines){
        printf("La chaine est vide \n");
        return NULL;
    }

    Reseau* R = (Reseau*)malloc(sizeof(Reseau));
    if (R == NULL) {
        printf("malloc pas marché\n");
        return NULL;
    }
    R->nbNoeuds = 0;
    R->gamma = C->gamma;
    R->noeuds = NULL;
    R->commodites = NULL;

    double* xmin;
    double* ymin; 
    double* xmax;
    double* ymax;
    chaineCoordMinMax(C, &xmin, &ymin, &xmax, &ymax);
    double coteX = xmax -xmin;
    double coteY = ymax - ymin;
    double xc = coteX/2;
    double yc = coteY/2;
    ArbreQuat* aq = creerArbreQuat(xc,yc,coteX,coteY);

    CellChaine* tempC = C->chaines;
    while(tempC){
        CellPoint* tempP = tempC->points;
        while(tempP){
            Noeud* noeud = rechercheCreeNoeudArbre(R,&aq,NULL,tempP->x,tempP->y);
            insererNoeudArbre(noeud,&aq,NULL);
            tempP = tempP->suiv;
        }
        tempC = tempC->suiv;
    }
}

/*
void ajouterCommoditeNoeuds(CellCommodite** commodites, Noeud* extrA, Noeud* extrB) {
    // Création d'une nouvelle commodité
    CellCommodite* nouvelleCommodite = (CellCommodite*)malloc(sizeof(CellCommodite));
    nouvelleCommodite->extrA = extrA;
    nouvelleCommodite->extrB = extrB;
    nouvelleCommodite->suiv = NULL;

    // Ajout de la commodité à la liste
    if (*commodites == NULL) {
        *commodites = nouvelleCommodite;
    } else {
        CellCommodite* temp = *commodites;
        while (temp->suiv != NULL) {
            temp = temp->suiv;
        }
        temp->suiv = nouvelleCommodite;
    }
}

void rechercherAjouterCommoditesArbre(CellCommodite** commodites, ArbreQuat* arbre, CellPoint* points, Reseau* R) {
    // Parcours de chaque point de la chaîne
    CellPoint* point = points;
    while (point != NULL) {
        // Recherche du nœud correspondant au point dans l'arbre quaternaire
        Noeud* noeud = rechercheCreeNoeudArbre(R, &arbre, NULL, point->x, point->y);
        if (noeud != NULL) {
            // Ajout du nœud à la liste des nœuds du réseau
            ajouterCommoditeNoeuds(commodites, noeud, noeud);
        }
        point = point->suiv;
    }
}

void rechercherAjouterCommoditesReseau(Reseau* R, Chaines* C) {
    if (R == NULL || C == NULL || C->chaines == NULL) {
        return;
    }

    // Parcours de chaque chaîne
    CellChaine* tempC = C->chaines;
    while (tempC != NULL) {
        // Rechercher et ajouter les commodités pour chaque chaîne
        rechercherAjouterCommoditesArbre(&(R->commodites), NULL, tempC->points, R);
        tempC = tempC->suiv;
    }
}


Reseau* reconstitueReseauArbre(Chaines* C) {
    if (C == NULL || C->nbChaines == 0) {
        printf("PPPPPPPPPPPPPPPPPPPPPPPPPPPPP\n");
        return NULL; // Si la liste des chaînes est vide, retourner NULL
    }

    // Création du réseau
    Reseau* R = (Reseau*)malloc(sizeof(Reseau));
    R->nbNoeuds = 0; // Initialisation du nombre de nœuds à zéro
    R->gamma = 0; // Initialisation du nombre maximal de fibres par câble à zéro
    R->noeuds = NULL; // Initialisation de la liste des nœuds à NULL
    R->commodites = NULL; // Initialisation de la liste des commodités à NULL
    
    // Création de l'arbre quaternaire
    ArbreQuat* arbre = NULL;
    
    // Parcours de chaque chaîne
    CellChaine* tempC = C->chaines;
    while (tempC) {
        // Parcours de chaque point de la chaîne
        CellPoint* point = tempC->points;
        while (point) {
            // Recherche ou création du nœud correspondant au point dans l'arbre quaternaire
            Noeud* noeud = rechercheCreeNoeudArbre(R, &arbre, NULL, point->x, point->y);
            // Ajout du nœud à la liste des noeuds du réseau
            CellNoeud* nouveauNoeud = (CellNoeud*)malloc(sizeof(CellNoeud));
            nouveauNoeud->nd = noeud;
            nouveauNoeud->suiv = R->noeuds;
            R->noeuds = nouveauNoeud;
            point = point->suiv; // Passage au point suivant dans la chaîne
        }
        tempC = tempC->suiv; // Passage à la chaîne suivante
    }

    // Recherche et ajout des commodités associées aux extrémités des chaînes
    rechercherAjouterCommoditesReseau(R, C);


    

    return R; // Retour du réseau reconstruit
}
*/

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
