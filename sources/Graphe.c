#include <stdlib.h>
#include <stdio.h>

#include "Graphe.h"
#include "Struct_File.h"
#define INT32_MAX 2147483647


Graphe* creerGraphe(Reseau* R) {
    if (R == NULL || R->nbNoeuds == 0) { // si vide
        printf("Erreur creerGraphe : Réseau NULL ou vide.\n");
        return NULL;//return direct
    }

    // Creation du graphe
    Graphe* G = (Graphe*)malloc(sizeof(Graphe)); 
    G->nbsom = R->nbNoeuds; 
    G->gamma = R->gamma; 
    G->T_som = (Sommet**)malloc(G->nbsom * sizeof(Sommet*)); // Alloue de la mémoire pour le tableau de pointeurs de sommets
    G->nbcommod = 0;
    G->T_commod = NULL;

    // Création des sommets du graphe
    CellNoeud* tempCN = R->noeuds;
    for (int i = 0; i < G->nbsom; i++) {
        G->T_som[i] = (Sommet*)malloc(sizeof(Sommet)); // Alloue de la mémoire pour un sommet
        G->T_som[i]->num = tempCN->nd->num; // Numéro du sommet
        G->T_som[i]->x = tempCN->nd->x; // Coordonnées x du sommet
        G->T_som[i]->y = tempCN->nd->y; // Coordonnées y du sommet
        G->T_som[i]->L_voisin = NULL; 
        tempCN = tempCN->suiv;
    }

    // Ajout des arêtes entre les sommets
    tempCN = R->noeuds;
    while (tempCN != NULL) {
        Noeud* nd = tempCN->nd;
        CellNoeud* tempVoisin = nd->voisins;
        while (tempVoisin != NULL) {
            int voisinNum = tempVoisin->nd->num; // Numéro du voisin
            // Recherche des sommets correspondants dans le graphe
            int index_nd = nd->num - 1; // Numéro du noeud - 1 pour correspondre à l'indice du tableau dans le graphe
            int index_voisin = voisinNum - 1; // Numéro du voisin - 1 pour correspondre à l'indice du tableau dans le graphe
            // Création de l'arête
            Arete* A = (Arete*)malloc(sizeof(Arete));
            A->u = G->T_som[index_nd]->num;
            A->v = G->T_som[index_voisin]->num;
            // Ajout de l'arête à la liste des voisins des sommets correspondants
            Cellule_arete* ca = (Cellule_arete*)malloc(sizeof(Cellule_arete));
            ca->a = A;
            ca->suiv = G->T_som[index_nd]->L_voisin;
            G->T_som[index_nd]->L_voisin = ca;
            // Passage au voisin suivant
            tempVoisin = tempVoisin->suiv;
        }
        // Passage au noeud suivant
        tempCN = tempCN->suiv;
    }

    // Création des sommets du graphe
    CellCommodite* tempCom = R->commodites;
    for (int i = 0; i < G->nbcommod; i++) {
        G->T_commod[i].e1 = tempCom->extrA->num;
        G->T_commod[i].e2 = tempCom->extrB->num;
        tempCom = tempCom->suiv;
    }

    return G; // Retourne le graphe créé
}

void liberer_graphe(Graphe* G) {
    if (G == NULL) {//si G est vide
        printf(" G est deja vide\n");
        return; // return direct
    }

    // on libère la mémoire allouée pour chaque sommet
    for (int i = 0; i < G->nbsom; i++) {
        Cellule_arete* temp = G->T_som[i]->L_voisin;
        while (temp != NULL) {
            Cellule_arete* temp2 = temp;
            temp = temp->suiv;
            free(temp2->a);
            free(temp2);
        }
        free(G->T_som[i]);
    }
    free(G->T_som); // on libère la mémoire allouée pour le tableau de pointeurs de sommets
    free(G->T_commod); // on libère la mémoire du tableau des commodités

    free(G); // Libère la mémoire du graphe 
}

int plusPetitNbAretes(Graphe* G, int u, int v) {
    // Tableau pour stocker les distances entre le sommet u et les autres sommets
    int dist[G->nbsom];
    // Initialise toutes les distances à une valeur maximale, sauf pour le sommet u
    for (int i = 0; i < G->nbsom; i++)
        dist[i] = INT32_MAX;
    dist[u] = 0;
    // File pour parcourir les sommets en largeur d'abord
    S_file *f = (S_file *)malloc(sizeof(S_file));
    Init_file(f);
    enfile(f, u);
    // on fait un parcours en largeur pour calculer les distances
    while (dist[v] == INT32_MAX) {
        int n = defile(f);
        Cellule_arete* temp = G->T_som[n]->L_voisin;
        while(temp != NULL) {
            int m;
            // détermine l'autre extrémité de l'arête
            if (n == temp->a->u) {
            m = temp->a->v;
            } else {
            m = temp->a->u;
            }
            // On met à jour la distance si le sommet n'a pas encore été visité
            if (dist[m] == INT32_MAX) {
                dist[m] = dist[n] + 1;
                //on ajoute le sommet à la file pour l'explorer plus tard
                enfile(f, m);
            }
            temp = temp->suiv;
        }
    }
    // on vide la file
    while (!estFileVide(f))
        defile(f);
    free(f);
    // on retourne la distance entre les sommets u et v
    return dist[v];
}

int* plusCourtChemin(Graphe* G, int u, int v) {
    // tableau pour stocker les distances entre le sommet u et les autres sommets
    int dist[G->nbsom];
    // tableau pour stocker les prédécesseurs de chaque sommet sur le chemin le plus court
    int pred[G->nbsom];
    // on initialise toutes les distances à une valeur maximale, sauf pour le sommet u
    for (int i = 0; i < G->nbsom; i++)
        dist[i] = INT32_MAX;
    dist[u] = 0;
    // File pour parcourir les sommets en largeur 
    S_file *f = (S_file *)malloc(sizeof(S_file));
    Init_file(f);
    enfile(f, u);
    //On parcours en largeur pour calculer les distances et les prédécesseurs
    while (dist[v] == INT32_MAX) {
        int n = defile(f);
        // parcourt les voisins du sommet actuel
        Cellule_arete* temp = G->T_som[n]->L_voisin;
        while(temp != NULL) {
            int m;
            // détermine l'autre extrémité de l'arête
            if (temp->a->u == n) {
            m = temp->a->v;
            } else {
            m = temp->a->u;
            }
            //On met à jour la distance et le prédécesseur si le sommet n'a pas encore été visité
            if (dist[m] == INT32_MAX) {
                dist[m] = dist[n] + 1;
                pred[m] = n;
                //on ajoute le sommet à la file pour l'explorer plus tard
                enfile(f, m);
            }
            temp = temp->suiv;
        }
    }
    // On vide la file
    while (!estFileVide(f))
        defile(f);
    free(f);
    // On alloue de la mémoire pour le tableau qui stockera le chemin
    int *chemin = (int *)malloc((dist[v]+1)*sizeof(int));
    int n = v;
    int i = 0;
    // On reconstitue le chemin en parcourant les prédécesseurs à partir du sommet v.
    while (n != u) {
        chemin[i++] = n;
        n = pred[n];
    }
    chemin[i] = u;// On ajoute le sommet u au début du chemin
    return chemin;
}

int reorganiseReseau(Reseau* R) {
    if (R == NULL) { // si R vide 
        printf("Erreur reorganiseReseau : R est vide\n");
        return -1;//return direct
    }
    // Création du graphe
    Graphe* G = creerGraphe(R);
    // Initialisation de la matrice de comptage des chaînes passant par chaque arête
    int** m = (int**)malloc(G->nbsom * sizeof(int*));
    for (int i = 0; i < G->nbsom; i++) {
        m[i] = (int*)malloc(G->nbsom * sizeof(int));
        // Initialiser les valeurs à zéro
        for (int j = 0; j < G->nbsom; j++) {
            m[i][j] = 0;
        }
    }
    
    // Calcul de la plus courte chaîne pour chaque commodité
    for (int i = 0; i < G->nbcommod; i++) {
        int ppna = plusPetitNbAretes(G, G->T_commod[i].e1, G->T_commod[i].e2);
        int* pcc = plusCourtChemin(G, G->T_commod[i].e1, G->T_commod[i].e2);
        for (int j = 0; j < ppna; j++) {
            m[pcc[j]][pcc[j+1]]++;
            m[pcc[j+1]][pcc[j]]++;
        }
        free(pcc);
    }
    
    // Vérification si le nombre de chaînes passant par chaque arête est inférieur à gamma
    for (int i = 0; i < G->nbsom; i++) {
        for (int j = 0; j < G->nbsom; j++) {
            // Pour chaque paire de sommets (i, j), où i et j sont les indices des sommets dans le graphe
            // Si le nombre de chaînes passant par l'arête (i, j) dépasse gamma
            if (m[i][j] > G->gamma) {
                // Libération de la mémoire allouée pour le graphe et la matrice
                for (int k = 0; k < G->nbsom; k++) {
                    free(m[k]);
                }
                free(m);
                liberer_graphe(G);
                return 1; // Faux, le réseau ne peut pas être réorganisé
            }
        }
    }
    
    // On libère la mémoire de la matrice
    for (int i = 0; i < G->nbsom; i++) {
        free(m[i]);
    }
    free(m);
    
    // On libère la mémoire du graphe
    liberer_graphe(G);
    
    return 0; // Vrai, le réseau peut être réorganisé
}



int main(){

    //5000_USA-road-d-NY.cha
    FILE* f = fopen("05000_USA-road-d-NY.cha", "r");
    Chaines* c = lectureChaines(f);
    fclose(f);

    Reseau* R = reconstitueReseauArbre(c);
    afficheReseauSVG(R,"R avant");
    printf("%d \n", reorganiseReseau(R));
    afficheReseauSVG(R,"R apres");
    Graphe* G = creerGraphe(R);
    liberer_chaines(c);
    liberer_reseau(R);
    liberer_graphe(G);

    //00014_burma.cha
    FILE* f2 = fopen("00014_burma.cha", "r");
    Chaines* c2 = lectureChaines(f2);
    fclose(f2);

    Reseau* R2 = reconstitueReseauArbre(c2);
    printf("%d \n", reorganiseReseau(R2));
    Graphe* G2 = creerGraphe(R2);
    liberer_chaines(c2);
    liberer_reseau(R2);
    liberer_graphe(G2);

    //07397_pla.cha
    FILE* f3 = fopen("07397_pla.cha", "r");
    Chaines* c3 = lectureChaines(f3);
    fclose(f3);

    Reseau* R3 = reconstitueReseauArbre(c3);
    printf("%d \n", reorganiseReseau(R3));
    reorganiseReseau(R3);
    Graphe* G3 = creerGraphe(R3);
    liberer_chaines(c3);
    liberer_reseau(R3);
    liberer_graphe(G3);

    return 0;
}
