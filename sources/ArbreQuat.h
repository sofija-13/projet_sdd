#ifndef __ARBRE_QUAT_H__
#define __ARBRE_QUAT_H__
#include "Reseau.h"

/* Arbre quaternaire contenant les noeuds du reseau */
typedef struct arbreQuat{
    double xc, yc;          /* Coordonnees du centre de la cellule */	
    double coteX;           /* Longueur de la cellule */
    double coteY;           /* Hauteur de la cellule */
    Noeud* noeud;           /* Pointeur vers le noeud du reseau */
    struct arbreQuat *so;   /* Sous-arbre sud-ouest, pour x < xc et y < yc */
    struct arbreQuat *se;   /* Sous-arbre sud-est, pour x >= xc et y < yc */
    struct arbreQuat *no;   /* Sous-arbre nord-ouest, pour x < xc et y >= yc */
    struct arbreQuat *ne;   /* Sous-arbre nord-est, pour x >= xc et y >= yc */
} ArbreQuat;

void chaineCoordMinMax(Chaines* C, double* xmin, double* ymin, double* xmax, double* ymax);
//détermine les coordonnées minimales et maximales des points constituant les différentes chaı̂nes du réseau.

ArbreQuat* creerArbreQuat(double xc, double yc, double coteX, double coteY);
/*permet de créer une cellule de l’arbre quaternaire, de centre (xc , yc ), de longueur
coteX et de hauteur coteY. Cette fonction initialisera le nœud du réseau, les arbres nord-ouest, nord-
est, sud-ouest et sud-est à NULL.*/

void insererNoeudArbre(Noeud* n, ArbreQuat** a, ArbreQuat *parent);
//insérer un nœud dans l'arbre quaternaire

Noeud* rechercheCreeNoeudArbre(Reseau* R, ArbreQuat** a, ArbreQuat *parent, double x, double y);
//rechercher ou créer un nœud dans l'arbre quaternaire

Reseau* reconstitueReseauArbre(Chaines* C);
//

void libererArbreQuaternaire(ArbreQuat* a);
#endif
