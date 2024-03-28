#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "Hachage.h"

double clef(double x, double y){ 
    return y + (x + y)*(x + y + 1)/2;
}

int hachage(double k, int m){
    double a = (sqrt(5.0)-1.0)/2.0; // nombre d'or -1
    double x = k * a - (int)(k * a);
    return (int)(x*m);
} // c'est la meme que celle du projet precedent
// attendre correction mini-projet pour valider

void ajoutNoeudHachage(TableHachage* H, double x, double y, int num){
    // allocation nouveau noeud
    CellNoeud *nouveau = (CellNoeud*)malloc(sizeof(CellNoeud)); 
    nouveau->nd = (Noeud*)malloc(sizeof(Noeud));
    nouveau->nd->num = num;
    nouveau->nd->x = x;
    nouveau->nd->y = y;
    nouveau->nd->voisins = NULL;

    // ajout dans la table de hachage
    int index = hachage(clef(x, y), H->tailleMax);
    nouveau->suiv = H->T[index];
    H->T[index] = nouveau;
}

Noeud* rechercheCreeNoeudHachage(Reseau* R, TableHachage* H, double x, double y){
    if (R==NULL || H==NULL){ // test validite arguments
        printf("Erreur rechercheCreeNoeudHachage : reseau ou table de hachage NULL\n");
        return NULL;
    }

    // recherche noeud correspondant au point (x, y) dans H
    int index = hachage(clef(x, y), H->tailleMax);
    if (H->T[index] && H->T[index]->nd){
        CellNoeud *temp = H->T[index];
        while(temp){
            if (temp->nd->x==x && temp->nd->y==y){
                return temp->nd;
            }
            temp = temp->suiv;  
        }
    }
    
    //ajout dans la table de hachage
    ajoutNoeudHachage(H, x, y, (R->nbNoeuds)+1);
    
    //creation nouveau CellNoeud
    CellNoeud *nouveau = (CellNoeud*)malloc(sizeof(CellNoeud)); 
    nouveau->nd = (Noeud*)malloc(sizeof(Noeud));
    nouveau->nd->num = (R->nbNoeuds)+1;
    nouveau->nd->x = x;
    nouveau->nd->y = y;
    nouveau->nd->voisins = NULL;

    // ajout en tete dans la liste des noeuds du reseau
    nouveau->suiv = R->noeuds;
    R->noeuds = nouveau;
    R->nbNoeuds++;

    return nouveau->nd;
}

// Reseau* reconstitueReseauHachage(Chaines *C, int M);

/*
int main(){
    // test 
    for (int x=1; x<11; x++){
        for (int y=1; y<11; y++){
            printf("clef(%d,%d)=%lf\n", x, y, clef(x, y));
        }
    }
    
    return 0;
}
*/