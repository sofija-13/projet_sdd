#ifndef __RESEAU_H__
#define __RESEAU_H__
#include "Chaine.h"

typedef struct noeud Noeud;

/* Liste chainee de noeuds (pour la liste des noeuds du reseau ET les listes des voisins de chaque noeud) */
typedef struct cellnoeud {
    Noeud *nd;      /* Pointeur vers le noeud stocke */
    struct cellnoeud *suiv; /* Cellule suivante dans la liste */
} CellNoeud;

/* Noeud du reseau */
struct noeud{
   int num;                        /* Numero du noeud */
   double x, y;                    /* Coordonnees du noeud*/
   CellNoeud *voisins;             /* Liste des voisins du noeud */
};

/* Liste chainee de commodites */
typedef struct cellCommodite {
    Noeud *extrA, *extrB;       /* Noeuds aux extremites de la commodite */
    struct cellCommodite *suiv;     /* Cellule suivante dans la liste */
} CellCommodite;

/* Un reseau */
typedef struct {
    int nbNoeuds;                   /* Nombre de noeuds du reseau */
    int gamma;                      /* Nombre maximal de fibres par cable */
    CellNoeud *noeuds;              /* Liste des noeuds du reseau */
    CellCommodite *commodites;      /* Liste des commodites a relier */
} Reseau;


Noeud* rechercheCreeNoeudListe(Reseau *R, double x, double y);
// retourne un Noeud du reseau R correspondant au point (x, y) s'il est dans la liste des noeuds de R, le cree sinon

int rechercheVoisin(Noeud *nd1, Noeud *nd2);
//regarde si nd2 est voisin de nd1

void ajouterVoisin(Noeud* noeud, Noeud* voisin);
//ajoute le voisin au noeud 

Reseau* reconstitueReseauListe(Chaines *C);
// reconstruit le reseau R a partir de la liste des chaınes C comme indique dans le pseudo-code

int nbLiaisons(Reseau *R);
// compte le nombre de liaisons dans le reseau

int nbCommodites(Reseau *R);
// compte le nombre de commodites dans le reseau

int nbVoisins(Noeud *nd);
// compte le nombre de voisin d'un noeud

void ecrireReseau(Reseau *R, FILE *f);
//

void afficheReseauSVG(Reseau *R, char* nomInstance);
//

void liberer_noeuds(CellNoeud *noeuds) ;
// libere la memoire des noeuds

void liberer_reseau(Reseau *R);
// libere la memoire allouee 

#endif

