#ifndef __CHAINE_H__
#define __CHAINE_H__	
#include <stdio.h>

/* Liste chainee de points */
typedef struct cellPoint{
  double x,y;                   /* Coordonnees du point */
  struct cellPoint *suiv;       /* Cellule suivante dans la liste */
} CellPoint;

/* Cellule d une liste (chainee) de chaines */
typedef struct cellChaine{
  int numero;                   /* Numero de la chaine */
  CellPoint *points;            /* Liste des points de la chaine */
  struct cellChaine *suiv;      /* Cellule suivante dans la liste */
} CellChaine;

/* L'ensemble des chaines */
typedef struct {
  int gamma;                    /* Nombre maximal de fibres par cable */
  int nbChaines;                /* Nombre de chaines */
  CellChaine *chaines;          /* La liste chainee des chaines */
} Chaines;

Chaines* lectureChaines(FILE *f);
//permet d’allouer, de remplir et de retourner une instance de notre structure a partir d’un fichier

void ecrireChaines(Chaines *C, FILE *f);
// ecrit dans un fichier le contenu d’une Chaines en respectant le format

void afficheChainesSVG(Chaines *C, char* nomInstance);
//permet de creer le fichier SVG en html a partir d’un struct Chaines

double longueurChaine(CellChaine *c);
//calcule la longueur physique d'une chaine

double longueurTotale(Chaines *C);
//calcule la longueur physique d'une chaine

int comptePointsTotal(Chaines *C);

#endif	
