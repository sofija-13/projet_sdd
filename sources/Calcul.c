#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Reseau.h"
#include "Hachage.h"
#include "ArbreQuat.h"

// pour la generation aleatoire de chaines
#define GAMMA 3 // nombre maximal de fibres par cable
#define NB_POINTS_CHAINE 10
#define X_MAX 100
#define Y_MAX 100

// pour les temps de calcul
#define TAILLE 100 // taille max de la table de hachage

Chaines* generationAleatoire(int nbChaines, int nbPointsChaine, int xmax, int ymax){
    /* permet de creer des chaines de points situes entre les points (0,0) et (xmax,ymax)
    nbChaines : nombre de chaines a creer
    nbPointsChaine : nombre de points par chaine*/

    // creation Chaines* qui va etre retournee
    Chaines *nv = (Chaines*)malloc(sizeof(Chaines));
    if (nv == NULL) {
        printf("Erreur malloc CellChaine* dans generationAleatoire\n");
        return NULL;
    }
    nv->gamma = GAMMA;
    nv->nbChaines = nbChaines;

    nv->chaines = NULL;  // initialisation liste des chaines
    for (int i = 0; i < nv->nbChaines; i++){
        // nouvelle chaine
        CellChaine *c = (CellChaine*)malloc(sizeof(CellChaine));
        if (c == NULL){
            printf("Erreur malloc CellChaine* dans generationAleatoire\n");
            return NULL;
        }
        c->numero = i;
        c->points = NULL;  // initialisation liste des points

        for (int j = 0; j < nbPointsChaine; j++){
            // nouveau point
            CellPoint *p = (CellPoint*)malloc(sizeof(CellPoint));
            if (p == NULL){
                printf("Erreur malloc CellPoint* dans generationAleatoire\n");
                return NULL;
            }
            // coordonnees aleatoires entre (0,0) et (xmax,ymax)
            p->x = (double)rand()/(RAND_MAX)*xmax;
            p->y = (double)rand()/(RAND_MAX)*ymax;
            // printf("x = %lf y = %lf\n", p->x, p->y);
            
            // ajout du point cree en tete de la liste des points
            p->suiv = c->points;
            c->points = p;
        }

        // ajout de la chaine cree en tete de la liste des chaines
        c->suiv = nv->chaines;
        nv->chaines = c;
    }

    return nv; 
}

/* Test fonction generationAleatoire
int main(){
    srand(time(NULL));
    FILE *f = fopen("test_ecriture.cha", "w");
    ecrireChaines(generationAleatoire(3, NB_POINTS_CHAINE, X_MAX, Y_MAX), f);
    fclose(f);
    return 0;
}
*/

int main(int argc, char** argv){
    if (argc != 2){
        printf("Usage : %s <méthode de test> \n", argv[0]);
        printf("Méthodes :\n");
        printf("0\tÀ partir du fichier 00014_burma.cha\n");
        printf("1\tAvec des chaines générées aléatoirement\n");
        return 1;
    }

    srand(time(NULL));
    Reseau* R = NULL;
    FILE* f = NULL;
    clock_t temps_initial = clock();
    clock_t temps_final = clock();
    
    switch(atoi(argv[1])){
    case 0: // fichier 00014_burma.cha

        // creation chaine a partir du fichier
        f = fopen("00014_burma.cha", "r");
        Chaines* c = lectureChaines(f);
        fclose(f);

        f = fopen("temps_fichier.txt","w");
        fprintf(f, "# nbPoints, liste_chainee, table_hachage, arbre_quaternaire\n");
        
        // liste chainee
        temps_initial = clock();
        R = reconstitueReseauListe(c);
        temps_final = clock();
        double temps_cpu = ((double)(temps_final - temps_initial)) / CLOCKS_PER_SEC;

        // table de hachage 
        temps_initial = clock();
        R = reconstitueReseauHachage(c, TAILLE);
        temps_final = clock();
        double temps_cpu2 = ((double)(temps_final - temps_initial)) / CLOCKS_PER_SEC;

        // arbre quaternaire
        temps_initial = clock();
        R = reconstitueReseauArbre(c);
        temps_final = clock();
        double temps_cpu3 = ((double)(temps_final - temps_initial)) / CLOCKS_PER_SEC;
        
        // ecriture valeurs calculees dans fichier
        fprintf(f,"%lf %lf %lf\n", temps_cpu, temps_cpu2, temps_cpu3);
        fclose(f);

        // liberation memoire allouee pour Chaines*
        liberer_chaines(c);
        break;
        
    case 1: // avec chaines generees aleatoirement

        // ouverture et ecriture 1ere ligne dans le fichier de sortie
        f = fopen("temps_calcul.csv","w"); // fichier pour table de hachage et arbre quaternaire
        fprintf(f, "# nbPointsTotal, liste_chainee, table_hachage, arbre_quaternaire\n");

        // evaluation des temps de calcul
        for (int i=500; i<=5000; i+=500){
            Chaines* c = generationAleatoire(i, NB_POINTS_CHAINE, X_MAX, Y_MAX);

            // liste chainee
            temps_initial = clock();
            R = reconstitueReseauListe(c);
            temps_final = clock();
            double temps_cpu = ((double)(temps_final - temps_initial)) / CLOCKS_PER_SEC;

            // table de hachage ------------------------ FAIRE VARIER LA TAILLE DE LA TABLE !! --------------
            temps_initial = clock();
            R = reconstitueReseauHachage(c, TAILLE);
            temps_final = clock();
            double temps_cpu2 = ((double)(temps_final - temps_initial)) / CLOCKS_PER_SEC;
            
            // arbre quaternaire
            temps_initial = clock();
            R = reconstitueReseauArbre(c);
            temps_final = clock();
            double temps_cpu3 = ((double)(temps_final - temps_initial)) / CLOCKS_PER_SEC;
            
            // ecriture dans fichier
            fprintf(f,"%d %lf %lf %lf\n", i*NB_POINTS_CHAINE, temps_cpu, temps_cpu2, temps_cpu3);

            liberer_chaines(c);
        }
        fclose(f);
    }

    // liberation memoire allouee pour Reseau 
    liberer_reseau(R);

    return 0;
}
