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

void ajoutNoeudHachage(TableHachage* H, Noeud* nv){
    CellNoeud *nouveau = (CellNoeud*) malloc(sizeof(CellNoeud)); 
    nouveau->nd = nv;
    
    int index = hachage(clef(nouveau->nd->x, nouveau->nd->y), H->tailleMax);
    
    nouveau->suiv = H->T[index];
    H->T[index] = nouveau;
}

Noeud* rechercheCreeNoeudHachage(Reseau* R, TableHachage* H, double x, double y){
    if (R==NULL || H==NULL){ // test validite arguments
        printf("Erreur rechercheCreeNoeudHachage : reseau ou table de hachage NULL\n");
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
    CellNoeud *nouveau = (CellNoeud*) malloc(sizeof(CellNoeud)); 
    nouveau->nd = (Noeud*)malloc(sizeof(Noeud));
    nouveau->nd->num = (R->nbNoeuds)+1;
    nouveau->nd->x = x;
    nouveau->nd->y = y;
    nouveau->nd->voisins = NULL;
    // ajout en tete dans la liste des noeuds du reseau
    nouveau->suiv = R->noeuds;
    R->noeuds = nouveau;
    R->nbNoeuds++;
    //ajout dans la table de hachage
    ajoutNoeudHachage(H, nouveau->nd);
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

void liberer_table_hachage(TableHachage* H) {
    if (H == NULL) {
        printf("Erreur liberer_table_hachage : la table de hachage est NULL\n");
        return;
    }
    // Libération de chaque liste chainée de la table de hachage
    for (int i = 0; i < H->tailleMax; i++) {
        CellNoeud *courant = H->T[i];
        while (courant != NULL) {
            CellNoeud *suivant = courant->suiv;
            free(courant);
            courant = suivant;
        }
    }
    // Libération du tableau de pointeurs
    free(H->T);
    // Libération de la structure TableHachage
    free(H);
}

Reseau* reconstitueReseauHachage(Chaines *C, int M){
    if (C == NULL || C->nbChaines == 0) { // test validite des arguments
        printf("Erreur reconstitueReseauHachage : C == NULL ou 0 chaine\n");
        return NULL;
    }
    // nouveau reseau
    Reseau* res = (Reseau*)malloc(sizeof(Reseau));
    if (res == NULL) {
        printf("Erreur reconstitueReseauHachage: erreur malloc pour nouveau reseau\n");
        return NULL;
    }
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
        } else {
            printf("Erreur reconstitueReseauHachage : échec malloc commodité\n");
            return NULL;
        }
        
        tempC = tempC->suiv;
    }
    
    liberer_table_hachage(H);
    
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


