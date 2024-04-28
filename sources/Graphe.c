#include <stdlib.h>
#include <stdio.h>

#include "Graphe.h"
#include "Struct_File.h"
#define INT32_MAX 2147483647


Graphe* creerGraphe(Reseau* r) {
    if (r == NULL || r->nbNoeuds == 0) { // Vérifie si le réseau est valide et non vide
        printf("Erreur creerGraphe : Réseau NULL ou vide.\n");
        return NULL;
    }

    // Alloue de la mémoire pour le graphe
    Graphe* graphe = (Graphe*)malloc(sizeof(Graphe)); 
    graphe->nbsom = r->nbNoeuds; // Nombre de sommets dans le graphe
    graphe->gamma = r->gamma; // Nombre maximal de fibres par câble
    graphe->T_som = (Sommet**)malloc(graphe->nbsom * sizeof(Sommet*)); // Alloue de la mémoire pour le tableau de pointeurs de sommets
    graphe->nbcommod = 0;
    graphe->T_commod = NULL;

    // Initialisation des sommets du graphe
    CellNoeud* tempCN = r->noeuds;
    for (int i = 0; i < graphe->nbsom; i++) {
        graphe->T_som[i] = (Sommet*)malloc(sizeof(Sommet)); // Alloue de la mémoire pour un sommet
        graphe->T_som[i]->num = tempCN->nd->num; // Numéro du sommet
        graphe->T_som[i]->x = tempCN->nd->x; // Coordonnées x du sommet
        graphe->T_som[i]->y = tempCN->nd->y; // Coordonnées y du sommet
        graphe->T_som[i]->L_voisin = NULL; // Initialise la liste des voisins à NULL
        tempCN = tempCN->suiv;
    }

    // Ajout des arêtes entre les sommets
    tempCN = r->noeuds;
    while (tempCN != NULL) {
        Noeud* nd = tempCN->nd;
        CellNoeud* tempVoisin = nd->voisins;
        while (tempVoisin != NULL) {
            int voisinNum = tempVoisin->nd->num; // Numéro du voisin
            // Recherche des sommets correspondants dans le graphe
            int index_nd = nd->num - 1; // Numéro du noeud - 1 pour correspondre à l'indice du tableau dans le graphe
            int index_voisin = voisinNum - 1; // Numéro du voisin - 1 pour correspondre à l'indice du tableau dans le graphe
            // Création de l'arête
            Arete* arete = (Arete*)malloc(sizeof(Arete));
            arete->u = graphe->T_som[index_nd]->num;
            arete->v = graphe->T_som[index_voisin]->num;
            // Ajout de l'arête à la liste des voisins des sommets correspondants
            Cellule_arete* cell = (Cellule_arete*)malloc(sizeof(Cellule_arete));
            cell->a = arete;
            cell->suiv = graphe->T_som[index_nd]->L_voisin;
            graphe->T_som[index_nd]->L_voisin = cell;
            // Passage au voisin suivant
            tempVoisin = tempVoisin->suiv;
        }
        // Passage au noeud suivant
        tempCN = tempCN->suiv;
    }

    CellCommodite* tempCommodite = r->commodites;
    for (int i = 0; i < graphe->nbcommod; i++) {
        graphe->T_commod[i].e1 = tempCommodite->extrA->num;
        graphe->T_commod[i].e2 = tempCommodite->extrB->num;
        tempCommodite = tempCommodite->suiv;
    }

    return graphe; // Retourne le graphe créé
}

void liberer_graphe(Graphe* g) {
    if (g == NULL) {
        return; // Vérifie si le graphe est NULL
    }

    // Libère la mémoire allouée pour chaque sommet
    for (int i = 0; i < g->nbsom; i++) {
        Cellule_arete* current = g->T_som[i]->L_voisin;
        while (current != NULL) {
            Cellule_arete* temp = current;
            current = current->suiv;
            free(temp->a);
            free(temp);
        }
        free(g->T_som[i]);
    }
    free(g->T_som); // Libère la mémoire allouée pour le tableau de pointeurs de sommets
    free(g->T_commod); // Libère la mémoire du tableau des commodités

    free(g); // Libère la mémoire du graphe 
}

int plusPetitNbAretes(Graphe* g, int u, int v) {
    int dist[g->nbsom];
    for (int i = 0; i < g->nbsom; i++)
        dist[i] = INT32_MAX;
    dist[u] = 0;
    S_file *f = (S_file *)malloc(sizeof(S_file));
    Init_file(f);
    enfile(f, u);
    while (dist[v] == INT32_MAX) {
        int n = defile(f);
        Cellule_arete* ca = g->T_som[n]->L_voisin;
        while(ca != NULL) {
            int m = ca->a->u == n ? ca->a->v : ca->a->u;
            if (dist[m] == INT32_MAX) {
                dist[m] = dist[n] + 1;
                enfile(f, m);
            }
            ca = ca->suiv;
        }
    }
    while (!estFileVide(f))
        defile(f);
    free(f);
    return dist[v];
}

int* plusCourtChemin(Graphe* g, int u, int v) {
    int dist[g->nbsom];
    int pred[g->nbsom];
    for (int i = 0; i < g->nbsom; i++)
        dist[i] = INT32_MAX;
    dist[u] = 0;
    S_file *f = (S_file *)malloc(sizeof(S_file));
    Init_file(f);
    enfile(f, u);
    while (dist[v] == INT32_MAX) {
        int n = defile(f);
        Cellule_arete* ca = g->T_som[n]->L_voisin;
        while(ca != NULL) {
            int m = ca->a->u == n ? ca->a->v : ca->a->u;
            if (dist[m] == INT32_MAX) {
                dist[m] = dist[n] + 1;
                pred[m] = n;
                enfile(f, m);
            }
            ca = ca->suiv;
        }
    }
    while (!estFileVide(f))
        defile(f);
    free(f);
    int *chemin = (int *)malloc((dist[v]+1)*sizeof(int));
    int n = v;
    int i = 0;
    while (n != u) {
        chemin[i++] = n;
        n = pred[n];
    }
    chemin[i] = u;
    return chemin;
}

int reorganiseReseau(Reseau* r) {
    // Création du graphe correspondant au réseau
    Graphe* g = creerGraphe(r);
    
    // Initialisation de la matrice de comptage des chaînes passant par chaque arête
    int** m = (int**)malloc(g->nbsom * sizeof(int*));
    if (m == NULL) {
        // Gestion de l'échec d'allocation
        liberer_graphe(g);
        return 0;
    }
    for (int i = 0; i < g->nbsom; i++) {
        m[i] = (int*)calloc(g->nbsom, sizeof(int));
        if (m[i] == NULL) {
            // Gestion de l'échec d'allocation
            for (int j = 0; j < i; j++) {
                free(m[j]);
            }
            free(m);
            liberer_graphe(g);
            return 0;
        }
    }
    
    // Calcul de la plus courte chaîne pour chaque commodité
    for (int i = 0; i < g->nbcommod; i++) {
        int ppna = plusPetitNbAretes(g, g->T_commod[i].e1, g->T_commod[i].e2);
        int* pcc = plusCourtChemin(g, g->T_commod[i].e1, g->T_commod[i].e2);
        if (pcc == NULL) {
            // Gestion de l'échec d'allocation
            for (int j = 0; j < g->nbsom; j++) {
                free(m[j]);
            }
            free(m);
            liberer_graphe(g);
            return 0;
        }
        for (int j = 0; j < ppna; j++) {
            m[pcc[j]][pcc[j+1]]++;
            m[pcc[j+1]][pcc[j]]++;
        }
        free(pcc);
    }
    
    // Vérification si le nombre de chaînes passant par chaque arête est inférieur à γ
    for (int i = 0; i < g->nbsom; i++) {
        for (int j = 0; j < g->nbsom; j++) {
            if (m[i][j] > g->gamma) {
                // Libération de la mémoire allouée pour le graphe et la matrice
                for (int k = 0; k < g->nbsom; k++) {
                    free(m[k]);
                }
                free(m);
                liberer_graphe(g);
                return 0; // Faux, le réseau ne peut pas être réorganisé
            }
        }
    }
    
    // Libération de la mémoire allouée pour la matrice
    for (int i = 0; i < g->nbsom; i++) {
        free(m[i]);
    }
    free(m);
    
    // Libération de la mémoire allouée pour le graphe
    liberer_graphe(g);
    
    return 1; // Vrai, le réseau peut être réorganisé
}



int main(){
    //
    FILE* f = fopen("05000_USA-road-d-NY.cha", "r");
    Chaines* c = lectureChaines(f);
    fclose(f);

    Reseau* R = reconstitueReseauArbre(c);
    afficheReseauSVG(R,"R avant");
    reorganiseReseau(R);
    afficheReseauSVG(R,"R apres");
    Graphe* G = creerGraphe(R);
    liberer_chaines(c);
    liberer_reseau(R);
    liberer_graphe(G);

    
    FILE* f2 = fopen("00014_burma.cha", "r");
    Chaines* c2 = lectureChaines(f2);
    fclose(f2);

    Reseau* R2 = reconstitueReseauArbre(c2);
    reorganiseReseau(R2);
    Graphe* G2 = creerGraphe(R2);
    liberer_chaines(c2);
    liberer_reseau(R2);
    liberer_graphe(G2);

    //
    FILE* f3 = fopen("07397_pla.cha", "r");
    Chaines* c3 = lectureChaines(f3);
    fclose(f3);

    Reseau* R3 = reconstitueReseauArbre(c3);
    reorganiseReseau(R3);
    Graphe* G3 = creerGraphe(R3);
    liberer_chaines(c3);
    liberer_reseau(R3);
    liberer_graphe(G3);

    return 0;
}
