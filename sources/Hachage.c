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
} 
/* c'est la meme que celle du projet precedent,
attendre correction mini-projet pour valider */

TableHachage* creer_hachage(int m){
    TableHachage* H = (TableHachage*) malloc(sizeof(TableHachage));
    if (H == NULL) { 
        // erreur malloc
        printf("Erreur creer_hachage : erreur malloc\n");
        return NULL;
    }

    // H->nbElement=0;
    H->tailleMax = m;
    H->T = (CellNoeud**) malloc(m * sizeof(CellNoeud*));
    if (H->T == NULL) { 
        // erreur malloc : liberation memoire H
        free(H);
        printf("Erreur creer_hachage : erreur malloc\n");
        return NULL;
    }
    // initialisation cases table hachage 
    for (int i = 0; i < m; i++) {
        H->T[i] = NULL;
    }
    return H;
} 

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

void liberer_noeud(CellNoeud* n){
    if (n == NULL){
        printf("liberer_noeud : noeud NULL\n");
        return;
    }
    CellNoeud* voisins = n->nd->voisins;
    while(voisins){
        CellNoeud* temp = voisins;
        voisins = voisins->suiv;
        liberer_noeud(temp);
    
    }
    free(n->nd->voisins);
    free(n->nd);
    free(n);
}

void liberer_hachage(TableHachage* H){
    if (H==NULL){ // test validite argument
        printf("Erreur liberer_hachage : H == NULL\n");
        return;
    }
    
    for (int i=0; i<(H->tailleMax); i++){
        CellNoeud* temp = H->T[i];
        while (temp){
            CellNoeud* temp2 = temp->suiv;
            liberer_noeud(temp);
            temp = temp2;
        }
    }
    free(H->T);
    free(H);
}

Reseau* reconstitueReseauHachage(Chaines *C, int M){
    if (C == NULL || C->nbChaines == 0) { // test validite des arguments
        printf("Erreur reconstitueReseauListe : C == NULL ou 0 chaine\n");
        return NULL;
    }

    // nouveau reseau
    Reseau* res = (Reseau*) malloc(sizeof(Reseau));
    if (res == NULL) {
        printf("Erreur reconstitueReseauListe: erreur malloc pour nouveau reseau\n");
        return NULL;
    }
    res->nbNoeuds = 0;
    res->gamma = C->gamma;
    res->noeuds = NULL;
    res->commodites = NULL;

    // "on utilise un ensemble de noeuds qui est initialise vide" (nouvelle table de hachage)
    TableHachage* H = creer_hachage(M);
    
    // "on parcourt une a une chaque chaine"
    CellChaine* tempC = C->chaines;
    while(tempC){
        // "pour chaque point p de la chaine"
        CellPoint* points = tempC->points;
        while(points){
            // "on ajoute un noeud correspondant au point" dans la table de hachage (s'il n'y est pas)
            Noeud* noeud = rechercheCreeNoeudHachage(res, H, points->x, points->y);
            
            // ajout du noeud en tete de la liste des noeuds du reseau
            CellNoeud* nouveau = (CellNoeud*) malloc(sizeof(CellNoeud));
            nouveau->nd = noeud;
            nouveau->suiv = res->noeuds;
            res->noeuds = nouveau;

            // "on met a jour la liste des voisins du point et celles de ses voisins"
            // ---------------------------------------

            points = points->suiv; 
        }
        // "on conserve la commodite de la chaine"
        // ---------------------------------------

        tempC = tempC->suiv;
    }

    return res;
}

/* test fonction clef
int main(){
    for (int x=1; x<11; x++){
        for (int y=1; y<11; y++){
            printf("clef(%d,%d)=%lf\n", x, y, clef(x, y));
        }
    }
    
    return 0;
}
*/