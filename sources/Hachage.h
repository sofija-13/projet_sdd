#ifndef __HACHAGE_H__
#define __HACHAGE_H__
#include "Reseau.h"

typedef struct{
  // int nbElement; //pas necessaire ici
  int tailleMax;
  CellNoeud** T;
} TableHachage ;

double clef(double x, double y);
// renvoie la clef associee a un point de coordonnees (x,y)

int hachage(double k, int m);
// renvoie l'index de la table de hachage associe a la clef k

void ajoutNoeudHachage(TableHachage* H, double x, double y, int num);
// ajoute un nouveau noeud de coordonnees (x,y) dans la table de hachage H

Noeud* rechercheCreeNoeudHachage(Reseau* R, TableHachage* H, double x, double y);
// retourne un Noeud du reseau R correspondant au point (x,y) dans la table de hachage H (ajoute si inexistant)

void liberer_hachage(TableHachage* H);
// libere la memoire allouee pour une TableHachage*

void liberer_noeud(CellNoeud* n);
// libere la memoire allouee pour un CellNoeud*

Reseau* reconstitueReseauHachage(Chaines *C, int M);
// reconstruit le reseau a partir de la liste des chaines C, en utilisant une table de hachage H de taille M

#endif
