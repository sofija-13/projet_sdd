#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "Hachage.h"

/*
double clef(double x, double y){ 
    return y + (x + y)*(x + y + 1)/2;
}

int hachage(double k, int m){
    double a = (sqrt(5.0)-1.0)/2.0; // nombre d'or -1
    double x = k * a - (int)(k * a);
    return (int)(x*m);
} 
*/
/* c'est la meme que celle du projet precedent,
attendre correction mini-projet pour valider */

/*
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
*//*
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

Noeud* rechercheCreeNoeudHachage(Reseau* R, TableHachage* H, double x, double y) {
    if (R == NULL || H == NULL) {
        printf("Erreur rechercheCreeNoeudHachage : reseau ou table de hachage NULL\n");
        return NULL;
    }

    // Calcule l'index dans la table de hachage
    int index = hachage(clef(x, y), H->tailleMax);
    
    // Vérifie si la liste chaînée à cet index est non vide
    if (H->T[index] != NULL) {
        // Parcourt la liste chaînée à cet index pour rechercher le noeud
        CellNoeud *temp = H->T[index];
        while (temp != NULL) {
            // Vérifie si les coordonnées correspondent
            if (temp->nd->x == x && temp->nd->y == y) {
                // Retourne le noeud si trouvé
                return temp->nd;
            }
            // Passe au noeud suivant dans la liste chaînée
            temp = temp->suiv;  
        }
    }
    
    // Si le noeud n'est pas trouvé dans la table de hachage, il faut le créer
    
    // Crée un nouveau noeud
    Noeud* nouveauNoeud = (Noeud*)malloc(sizeof(Noeud));
    if (nouveauNoeud == NULL) {
        printf("Erreur rechercheCreeNoeudHachage : échec malloc pour nouveauNoeud\n");
        return NULL;
    }
    // Initialise les valeurs du nouveau noeud
    nouveauNoeud->num = R->nbNoeuds + 1;
    nouveauNoeud->x = x;
    nouveauNoeud->y = y;
    nouveauNoeud->voisins = NULL;

    // Ajoute le nouveau noeud à la liste des noeuds du réseau
    CellNoeud* nouveauCellule = (CellNoeud*)malloc(sizeof(CellNoeud)); 
    if (nouveauCellule == NULL) {
        printf("Erreur rechercheCreeNoeudHachage : échec malloc pour nouveauCellule\n");
        free(nouveauNoeud); // Libère la mémoire allouée pour le nouveauNoeud
        return NULL;
    }
    nouveauCellule->nd = nouveauNoeud;
    nouveauCellule->suiv = R->noeuds;
    R->noeuds = nouveauCellule;
    R->nbNoeuds++;
    
    // Ajoute le nouveau noeud à la table de hachage
    ajoutNoeudHachage(H, nouveauNoeud);

    // Retourne le nouveau noeud créé
    return nouveauNoeud;
}



void ajoutNoeudHachage(TableHachage* H, Noeud* nv){
    if (H == NULL || nv == NULL) { 
        printf("Erreur ajoutNoeudHachage : H ou nv est NULL\n");
        return;
    }

    CellNoeud *nouveau = (CellNoeud*) malloc(sizeof(CellNoeud)); 
    if (nouveau == NULL) {
        printf("Erreur ajoutNoeudHachage : échec malloc pour nouveau\n");
        return;
    }

    nouveau->nd = nv;
    
    int index = hachage(clef(nv->x, nv->y), H->tailleMax);
    
    nouveau->suiv = H->T[index];
    H->T[index] = nouveau;
}

*/
/*
Noeud* rechercheCreeNoeudHachage(Reseau* R, TableHachage* H, double x, double y) {
    if (R == NULL || H == NULL) {
        printf("Erreur rechercheCreeNoeudHachage : reseau ou table de hachage NULL\n");
        return NULL;
    }

    // Calcule l'index dans la table de hachage
    int index = hachage(clef(x, y), H->tailleMax);
    
    // Vérifie si la liste chaînée à cet index est non vide
    if (H->T[index] != NULL) {
        // Parcourt la liste chaînée à cet index pour rechercher le noeud
        CellNoeud *temp = H->T[index];
        while (temp != NULL) {
            // Vérifie si les coordonnées correspondent
            if (temp->nd->x == x && temp->nd->y == y) {
                // Retourne le noeud si trouvé
                return temp->nd;
            }
            // Passe au noeud suivant dans la liste chaînée
            temp = temp->suiv;  
        }
    }
    
    // Si le noeud n'est pas trouvé dans la table de hachage, il faut le créer
    
    // Crée un nouveau noeud
    Noeud* nouveauNoeud = (Noeud*)malloc(sizeof(Noeud));
    if (nouveauNoeud == NULL) {
        printf("Erreur rechercheCreeNoeudHachage : échec malloc pour nouveauNoeud\n");
        return NULL;
    }
    // Initialise les valeurs du nouveau noeud
    nouveauNoeud->num = R->nbNoeuds + 1;
    nouveauNoeud->x = x;
    nouveauNoeud->y = y;
    nouveauNoeud->voisins = NULL;

    // Ajoute le nouveau noeud à la liste des noeuds du réseau
    CellNoeud* nouveauCellule = (CellNoeud*)malloc(sizeof(CellNoeud)); 
    if (nouveauCellule == NULL) {
        printf("Erreur rechercheCreeNoeudHachage : échec malloc pour nouveauCellule\n");
        free(nouveauNoeud); // Libère la mémoire allouée pour le nouveauNoeud
        return NULL;
    }
    nouveauCellule->nd = nouveauNoeud;
    nouveauCellule->suiv = R->noeuds;
    R->noeuds = nouveauCellule;
    R->nbNoeuds++;
    
    // Ajoute le nouveau noeud à la table de hachage
    ajoutNoeudHachage(H, nouveauNoeud);

    // Retourne le nouveau noeud créé
    return nouveauNoeud;
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
*/

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


int clef(int x, int y){
    return y + (x + y)*(x + y + 1)/2;
}

int hachage(int cle, int M){
    double A = (sqrt(5) - 1)/2;
    return (int)(M*(A*cle - (int)(A*cle)));
}

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

Noeud* rechercheCreeNoeudHachage(Reseau* R, TableHachage*H, double x, double y){
    printf("ICI1\n");
    int h = hachage(clef(x,y),H->tailleMax);
    CellNoeud* pcn = H->T[h];
    while(pcn != NULL){
        if(pcn->nd->x == x && pcn->nd->y == y)
        printf("ICI2\n");
            return pcn->nd;
        pcn = pcn->suiv;    
    }
    printf("ICI3\n");
    CellNoeud* nouv = (CellNoeud*)malloc(sizeof(CellNoeud));
    nouv->nd = (Noeud*)malloc(sizeof(Noeud));
    nouv->nd->num = R->nbNoeuds;
    R->nbNoeuds++;
    nouv->nd->voisins = NULL;
    nouv->nd->x = x;
    nouv->nd->y = y;
    nouv->suiv = H->T[h];
    H->T[h] = nouv;
    CellNoeud* nouv2 = (CellNoeud*)malloc(sizeof(CellNoeud));
    nouv2->nd = nouv->nd;
    nouv2->suiv = R->noeuds;
    R->noeuds = nouv2;
    printf("ICI4\n");
    return nouv->nd;
}

void liberer_table_hachage(TableHachage* h){
    CellNoeud** tab = h->T;
    for(int i = 0; i < h->tailleMax; i++){
        CellNoeud* pcn = h->T[i];
        while(pcn != NULL){
            CellNoeud* tmp = pcn;
            pcn = pcn->suiv;
            free(tmp);
        }
    }
    free(tab);
    free(h);
}

Reseau* reconstitueReseauHachage(Chaines *C, int M) {
    // Création de la table de hachage
    TableHachage* h = creer_hachage(M);
    if (h == NULL) {
        printf("Erreur reconstitueReseauHachage : erreur lors de la création de la table de hachage\n");
        return NULL;
    }
    printf("ICI5\n");
    // Création du réseau
    Reseau* r = (Reseau*)malloc(sizeof(Reseau));
    if (r == NULL) {
        printf("Erreur reconstitueReseauHachage : erreur d'allocation de mémoire pour le réseau\n");
        liberer_table_hachage(h);
        return NULL;
    }
    r->gamma = C->gamma;
    r->nbNoeuds = 0;
    r->noeuds = NULL;
    r->commodites = NULL;

    // Parcours des chaines
    CellChaine* pcc = C->chaines;
    while (pcc != NULL) {
        CellPoint* pcp = pcc->points;
        if (pcp != NULL) {
            CellCommodite* cmdt = (CellCommodite*)malloc(sizeof(CellCommodite));
            if (cmdt == NULL) {
                printf("Erreur reconstitueReseauHachage : erreur d'allocation de mémoire pour la commodité\n");
                liberer_table_hachage(h);
                free(r);
                return NULL;
            }
            Noeud* pn1 = rechercheCreeNoeudHachage(r, h, pcp->x, pcp->y);
            cmdt->extrA = pn1;

            while (pcp->suiv != NULL) {
                Noeud* pn2 = rechercheCreeNoeudHachage(r, h, pcp->suiv->x, pcp->suiv->y);
                if (rechercheVoisin(pn1, pn2) == 0) {
                    // Allocation des cellules de voisins
                    CellNoeud* pcn1 = (CellNoeud*)malloc(sizeof(CellNoeud));
                    CellNoeud* pcn2 = (CellNoeud*)malloc(sizeof(CellNoeud));
                    if (pcn1 == NULL || pcn2 == NULL) {
                        printf("Erreur reconstitueReseauHachage : erreur d'allocation de mémoire pour les cellules de voisins\n");
                        liberer_table_hachage(h);
                        free(cmdt);
                        free(r);
                        return NULL;
                    }

                    // Allocation des nœuds
                    pcn1->nd = (Noeud*)malloc(sizeof(Noeud));
                    pcn2->nd = (Noeud*)malloc(sizeof(Noeud));
                    if (pcn1->nd == NULL || pcn2->nd == NULL) {
                        printf("Erreur reconstitueReseauHachage : erreur d'allocation de mémoire pour les nœuds\n");
                        liberer_table_hachage(h);
                        free(cmdt);
                        free(r);
                        free(pcn1);
                        free(pcn2);
                        return NULL;
                    }

                    // Initialisation des nœuds
                    pcn1->nd->num = pn2->num;
                    pcn1->nd->x = pn2->x;
                    pcn1->nd->y = pn2->y;
                    pcn1->nd->voisins = NULL; // Le champ voisins doit être initialisé à NULL
                    pcn2->nd->num = pn1->num;
                    pcn2->nd->x = pn1->x;
                    pcn2->nd->y = pn1->y;
                    pcn2->nd->voisins = NULL; // Le champ voisins doit être initialisé à NULL

                    // Ajout des nœuds dans la liste des nœuds du réseau
                    pcn1->suiv = r->noeuds;
                    r->noeuds = pcn1;
                    pcn2->suiv = r->noeuds;
                    r->noeuds = pcn2;

                    // Ajout des cellules de voisins
                    pcn1->suiv = pn1->voisins;
                    pn1->voisins = pcn1;
                    pcn2->suiv = pn2->voisins;
                    pn2->voisins = pcn2;
                }
                pn1 = pn2;
                pcp = pcp->suiv;
            }
            cmdt->extrB = rechercheCreeNoeudHachage(r, h, pcp->x, pcp->y);
            cmdt->suiv = r->commodites;
            r->commodites = cmdt;
        }
        pcc = pcc->suiv;
    }

    liberer_table_hachage(h);
    printf("ICI999\n");
    return r;
}
