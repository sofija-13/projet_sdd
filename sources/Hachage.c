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
    double x = k * a - floor(k * a);
    return floor(x*m);
    
} 
/* c'est la meme que celle du projet precedent,
attendre correction mini-projet pour valider */


TableHachage* creer_hachage(int m){
    TableHachage* H = (TableHachage*) malloc(sizeof(TableHachage));

    // H->nbElement=0;
    H->tailleMax = m;
    H->T = (CellNoeud**) malloc(m * sizeof(CellNoeud*));
    // initialisation cases table hachage 
    for (int i = 0; i < m; i++) {
        H->T[i] = NULL;
    }
    return H;
} 

void ajoutNoeudHachage(TableHachage* H, Noeud* nv){
    CellNoeud *temp = (CellNoeud*) malloc(sizeof(CellNoeud)); 
    temp->nd = nv;
    int index = hachage(clef(temp->nd->x, temp->nd->y), H->tailleMax);
    temp->suiv = H->T[index];
    H->T[index] = temp;
}

Noeud* rechercheCreeNoeudHachage(Reseau* R, TableHachage* H, double x, double y){
    if (R==NULL || H==NULL){ // test validite arguments
        printf("Erreur rechercheCreeNoeudHachage : reseau ou table de hachage vide\n");
        return NULL;
    }
    
    // recherche noeud de coordonnees (x,y) dans H
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
    //creation nouveau CellNoeud
    CellNoeud *nv = (CellNoeud*) malloc(sizeof(CellNoeud)); 
    nv->nd = (Noeud*)malloc(sizeof(Noeud));
    nv->nd->num = (R->nbNoeuds)+1;
    nv->nd->x = x;
    nv->nd->y = y;
    nv->nd->voisins = NULL;
    // ajout en tete dans la liste des noeuds du reseau
    nv->suiv = R->noeuds;
    R->noeuds = nv;
    R->nbNoeuds++;
    //ajout dans la table de hachage
    ajoutNoeudHachage(H, nv->nd);
    return nv->nd;
}




Reseau* reconstitueReseauHachage(Chaines *C, int M){
    if (C == NULL || C->nbChaines == 0) { // test validite des arguments
        printf("Erreur reconstitueReseauHachage : C == NULL ou 0 chaine\n");
        return NULL;
    }
    // nouveau reseau
    Reseau* res = (Reseau*)malloc(sizeof(Reseau));
    res->nbNoeuds = 0;
    res->gamma = C->gamma;
    res->noeuds = NULL;
    res->commodites = NULL;

    // nouvelle table de hachage
    TableHachage* H = creer_hachage(M);
    // "on parcourt une a une chaque chaine"
    CellChaine *tempC = C->chaines;
    while(tempC){
        CellPoint *tempP = tempC->points;
        // premier noeud de la chaine
        Noeud* n1 = rechercheCreeNoeudHachage(res, H, tempP->x, tempP->y);
        // "pour chaque point de la chaine"
        while (tempP->suiv){
            // noeud suivant
            Noeud* n2 = rechercheCreeNoeudHachage(res, H, tempP->suiv->x, tempP->suiv->y);
            
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
            tempCom->extrB = rechercheCreeNoeudHachage(res, H, tempC->points->x, tempC->points->y);
            tempCom->suiv = res->commodites;
            res->commodites = tempCom;
        }
        
        tempC = tempC->suiv;
    }
    
    liberer_TableHachage(H);
    
    return res;
}

void liberer_TableHachage(TableHachage* H) {
    if (H == NULL) {
        printf(" H est deja vide\n");
        return;
    }
    // Libération de chaque liste chainée de la table de hachage
    for (int i = 0; i < H->tailleMax; i++) {
        CellNoeud *temp = H->T[i];
        while (temp != NULL) {
            CellNoeud *suivant = temp->suiv;
            free(temp);
            temp = suivant;
        }
    }
    // Libération du tableau de pointeurs
    free(H->T);
    // Libération de la structure TableHachage
    free(H);
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


