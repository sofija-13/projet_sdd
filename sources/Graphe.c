#include <stdlib.h>
#include <stdio.h>
#include "Graphe.h"

// A REVOIR.... NE MARCHE PAS
/*
Graphe* creerGraphe(Reseau* r) {
    if (r == NULL || r->nbNoeuds == 0) { // Vérifie si le réseau est valide et non vide
        printf("Erreur creerGraphe : Réseau NULL ou vide.\n");
        return NULL;
    }
    // Alloue de la mémoire pour le graphe
    Graphe* graphe = (Graphe*)malloc(sizeof(Graphe)); 
    graphe->nbsom = r->nbNoeuds; // Nombre de sommets dans le graphe
    graphe->gamma = r->gamma; // Nombre maximal de fibres par câble
    graphe->T_som = (Noeud**)malloc(graphe->nbsom * sizeof(Noeud*)); // Alloue de la mémoire pour le tableau de pointeurs de noeuds
    graphe->nbcommod = 0 ;
    graphe->T_commod = NULL;

    CellNoeud* tempCN = r->noeuds;
    while(tempCN){
        Noeud* tempN = tempCN->nd;

    }



    return graphe; // Retourne le graphe créé
}

void libererGraphe(Graphe* g) {
    if (g == NULL) {
        return; // Vérifie si le graphe est NULL
    }

}
*/

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
    /*
    // Libère la mémoire allouée pour chaque commodité
    for (int i = 0; i < g->nbcommod; i++) {
        // Vous pouvez ajouter des opérations de libération supplémentaires si nécessaire pour la structure Commod
    }*/
    free(g->T_commod); // Libère la mémoire du tableau des commodités

    free(g); // Libère la mémoire du graphe 
}

//bordure; sommet qu'on a rencontré mais qu'on a pas encore traité
int PlusPetitNbArete (Sommet* u, Sommet* v){
    int nb = 0;


    return nb;
}



int main(){

    FILE* f = fopen("00014_burma.cha", "r");
    Chaines* c = lectureChaines(f);
    fclose(f);

    Reseau* R = reconstitueReseauListe(c);

    Graphe* G = creerGraphe(R);
    liberer_chaines(c);
    liberer_reseau(R);
    liberer_graphe(G);
    return 0;
}

