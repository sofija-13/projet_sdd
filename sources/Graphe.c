#include <stdlib.h>
#include <stdio.h>
#include "Graphe.h"

// A REVOIR.... NE MARCHE PAS

Graphe* creerGraphe(Reseau* r) {
    if (r == NULL || r->nbNoeuds == 0) { // Vérifie si le réseau est valide et non vide
        printf("Erreur creerGraphe : Réseau NULL ou vide.\n");
        return NULL;
    }
    // Alloue de la mémoire pour le graphe
    Graphe* graphe = (Graphe*)malloc(sizeof(Graphe)); 
    if (graphe == NULL) {
        printf("Erreur creerGraphe : Impossible d'allouer de la mémoire pour le graphe.\n");
        return NULL;
    }
    graphe->nbsom = r->nbNoeuds; // Nombre de sommets dans le graphe
    graphe->gamma = r->gamma; // Nombre maximal de fibres par câble
    graphe->T_som = (Noeud**)malloc(graphe->nbsom * sizeof(Noeud*)); // Alloue de la mémoire pour le tableau de pointeurs de noeuds
    if (graphe->T_som == NULL) {
        printf("Erreur creerGraphe : Impossible d'allouer de la mémoire pour le tableau de noeuds.\n");
        free(graphe); // Libère la mémoire pour le graphe
        return NULL;
    }

    // Initialisation des noeuds dans le graphe en utilisant les noeuds du réseau
    CellNoeud* currentNoeud = r->noeuds; // Pointeur vers le premier noeud du réseau
    for (int i = 0; i < graphe->nbsom; i++) {
        graphe->T_som[i] = currentNoeud->nd; // Affecte directement le pointeur de noeud
        currentNoeud = currentNoeud->suiv; // Passe au noeud suivant dans le réseau
    }

    // Parcours des commodités pour ajouter les arêtes au graphe
    CellCommodite* currentCommodite = r->commodites;
    while (currentCommodite != NULL) {
        // Les extrémités de la commodité sont déjà des voisins
        ajouterVoisin(graphe->T_som[currentCommodite->extrA->num], graphe->T_som[currentCommodite->extrB->num]);
        ajouterVoisin(graphe->T_som[currentCommodite->extrB->num], graphe->T_som[currentCommodite->extrA->num]);
        currentCommodite = currentCommodite->suiv; // Passe à la commodité suivante
    }

    return graphe; // Retourne le graphe créé
}

void libererGraphe(Graphe* g) {
    if (g == NULL) {
        return; // Vérifie si le graphe est NULL
    }

    // Libération de la mémoire pour chaque sommet du graphe
    for (int i = 0; i < g->nbsom; i++) {
        Sommet* sommet = g->T_som[i];
        Cellule_arete* current = sommet->L_voisin;
        while (current != NULL) {
            Cellule_arete* temp = current;
            current = current->suiv;
            free(temp->a); // Libère la mémoire pour l'arête
            free(temp); // Libère la mémoire pour la cellule
        }
        free(sommet); // Libère la mémoire pour le sommet
    }
    free(g->T_som); // Libère la mémoire pour le tableau de sommets
    free(g); // Libère la mémoire pour le graphe
}

int main(){

    FILE* f = fopen("00014_burma.cha", "r");
    Chaines* c = lectureChaines(f);
    fclose(f);

    Reseau* R = reconstitueReseauListe(c);

    Graphe* G = creerGraphe(R);
    liberer_reseau(R);
    libererGraphe(G);
    return 0;
}

