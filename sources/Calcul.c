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
    nv->gamma = GAMMA;
    nv->nbChaines = nbChaines;

    nv->chaines = NULL;  // initialisation liste des chaines
    for (int i = 0; i < nv->nbChaines; i++){
        // nouvelle chaine
        CellChaine *tempC = (CellChaine*)malloc(sizeof(CellChaine));
        tempC->numero = i;
        tempC->points = NULL;  // initialisation liste des points

        for (int j = 0; j < nbPointsChaine; j++){
            // nouveau point
            CellPoint *tempP = (CellPoint*)malloc(sizeof(CellPoint));
            // coordonnees aleatoires entre (0,0) et (xmax,ymax)
            tempP->x = (double)rand()/(RAND_MAX)*xmax;
            tempP->y = (double)rand()/(RAND_MAX)*ymax;
            // printf("x = %lf y = %lf\n", p->x, p->y);
            
            // ajout du point cree en tete de la liste des points
            tempP->suiv = tempC->points;
            tempC->points = tempP;
        }

        // ajout de la chaine cree en tete de la liste des chaines
        tempC->suiv = nv->chaines;
        nv->chaines = tempC;
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
/*
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
}*/

/*    Plus on augmente la taille de la table de hachage et plus elle prend de temps à se terminer*/
int main(){
    srand(time(NULL));
    Reseau* R = NULL;
    FILE* f = NULL;
    clock_t temps_initial = clock();
    clock_t temps_final = clock();

                                            /*     00014_burma.cha     */
                                            
    // creation chaine a partir du fichier
    f = fopen("00014_burma.cha", "r");
    Chaines* c = lectureChaines(f);
    fclose(f);

    f = fopen("temps_fichier.txt","w");
    fprintf(f, "liste_chainee, table_hachage, arbre_quaternaire\n");
        
    // liste chainee
    temps_initial = clock();
    R = reconstitueReseauListe(c);
    temps_final = clock();
    double temps_cpu = ((double)(temps_final - temps_initial)) / CLOCKS_PER_SEC;
    liberer_reseau(R);

    // table de hachage 
    temps_initial = clock();
    R = reconstitueReseauHachage(c, TAILLE);
    temps_final = clock();
    double temps_cpu2 = ((double)(temps_final - temps_initial)) / CLOCKS_PER_SEC;
    liberer_reseau(R);

    // arbre quaternaire
    temps_initial = clock();
    R = reconstitueReseauArbre(c);
    temps_final = clock();
    double temps_cpu3 = ((double)(temps_final - temps_initial)) / CLOCKS_PER_SEC;
    liberer_reseau(R);

    // ecriture valeurs calculees dans fichier
    fprintf(f,"%lf %lf %lf\n", temps_cpu, temps_cpu2, temps_cpu3);
    // liberation memoire allouee pour chaine c
    liberer_chaines(c);
    //fclose(f);
    
                                            /*     05000_USA-road-d-NY.cha     */
                                           
    // creation chaine a partir du fichier
    FILE * f2 = fopen("05000_USA-road-d-NY.cha", "r");
    Chaines* c2 = lectureChaines(f2);
    fclose(f2);

    // liste chainee
    temps_initial = clock();
    Reseau * R2 = reconstitueReseauListe(c2);
    temps_final = clock();
    temps_cpu = ((double)(temps_final - temps_initial)) / CLOCKS_PER_SEC;
    liberer_reseau(R2);

    // table de hachage 
    temps_initial = clock();
    R2 = reconstitueReseauHachage(c2, TAILLE);
    temps_final = clock();
    temps_cpu2 = ((double)(temps_final - temps_initial)) / CLOCKS_PER_SEC;
    liberer_reseau(R2);

    // arbre quaternaire
    temps_initial = clock();
    R2 = reconstitueReseauArbre(c2);
    temps_final = clock();
    temps_cpu3 = ((double)(temps_final - temps_initial)) / CLOCKS_PER_SEC;
    liberer_reseau(R2);
    // ecriture valeurs calculees dans fichier
    fprintf(f,"%lf %lf %lf\n", temps_cpu, temps_cpu2, temps_cpu3);
    // liberation memoire allouee pour Chaines*
    liberer_chaines(c2);
    //fclose(f);
    

                                                /*     07397_pla.cha     */
                                                
    // creation chaine a partir du fichier
    FILE * f3 = fopen("07397_pla.cha", "r");
    Chaines* c3 = lectureChaines(f3);
    fclose(f3);
  
    // liste chainee
    temps_initial = clock();
    Reseau * R3 = reconstitueReseauListe(c3);
    temps_final = clock();
    temps_cpu = ((double)(temps_final - temps_initial)) / CLOCKS_PER_SEC;
    liberer_reseau(R3);

    // table de hachage 
    temps_initial = clock();
    R3 = reconstitueReseauHachage(c3, TAILLE);
    temps_final = clock();
    temps_cpu2 = ((double)(temps_final - temps_initial)) / CLOCKS_PER_SEC;
    liberer_reseau(R3);

    // arbre quaternaire
    temps_initial = clock();
    R3 = reconstitueReseauArbre(c3);
    temps_final = clock();
    temps_cpu3 = ((double)(temps_final - temps_initial)) / CLOCKS_PER_SEC;
    liberer_reseau(R3);
    // ecriture valeurs calculees dans fichier
    fprintf(f,"%lf %lf %lf\n", temps_cpu, temps_cpu2, temps_cpu3);
    fclose(f);
    // liberation de la memoire des chaines et réseau
    liberer_chaines(c3);
    

    /*     Quand la chaine est généré aléatoirement       */
    
    // ouverture et ecriture 1ere ligne dans le fichier de sortie
    FILE * f4 = fopen("temps_calcul.csv","w"); // fichier pour table de hachage et arbre quaternaire
    fprintf(f4, "# nbPointsTotal, liste_chainee, table_hachage, arbre_quaternaire\n");

    // evaluation des temps de calcul
    for (int i=500; i<=5000; i+=500){
        c = generationAleatoire(i, NB_POINTS_CHAINE, X_MAX, Y_MAX);

        // liste chainee
        temps_initial = clock();
        R = reconstitueReseauListe(c);
        temps_final = clock();
        double temps_cpu = ((double)(temps_final - temps_initial)) / CLOCKS_PER_SEC;
        liberer_reseau(R);

        // table de hachage ------------------------ FAIRE VARIER LA TAILLE DE LA TABLE !! --------------
        temps_initial = clock();
        R = reconstitueReseauHachage(c, TAILLE);
        temps_final = clock();
        double temps_cpu2 = ((double)(temps_final - temps_initial)) / CLOCKS_PER_SEC;
        liberer_reseau(R);
        
        // arbre quaternaire
        temps_initial = clock();
        R = reconstitueReseauArbre(c);
        temps_final = clock();
        double temps_cpu3 = ((double)(temps_final - temps_initial)) / CLOCKS_PER_SEC;
        liberer_reseau(R);

        // ecriture dans fichier
        fprintf(f4,"%d %lf %lf %lf\n", i*NB_POINTS_CHAINE, temps_cpu, temps_cpu2, temps_cpu3);
        liberer_chaines(c);
    }
    fclose(f4);
    

    /*  Taille de la table de Hachage varie  */

    FILE * fH1 = fopen("temps_hachage1.txt","w");
    fprintf(fH1, "taille 00014_burma.cha 05000_USA-road-d-NY.cha 07397_pla.cha\n");

    FILE * fh1 = fopen("00014_burma.cha", "r");
    Chaines* C1 = lectureChaines(fh1);
    fclose(fh1);

    FILE * fh2 = fopen("05000_USA-road-d-NY.cha", "r");
    Chaines* C2 = lectureChaines(fh2);
    fclose(fh2);

    FILE * fh3 = fopen("07397_pla.cha", "r");
    Chaines* C3 = lectureChaines(fh3);
    fclose(fh3);
    
    for (int i=500; i<=50000; i+=5000){

        // liste chainee
        temps_initial = clock();
        R = reconstitueReseauHachage(C1, i);
        temps_final = clock();
        double temps_cpu = ((double)(temps_final - temps_initial)) / CLOCKS_PER_SEC;
        liberer_reseau(R);

        // table de hachage ------------------------ FAIRE VARIER LA TAILLE DE LA TABLE !! --------------
        temps_initial = clock();
        R = reconstitueReseauHachage(C2, i);
        temps_final = clock();
        double temps_cpu2 = ((double)(temps_final - temps_initial)) / CLOCKS_PER_SEC;
        liberer_reseau(R);
        
        // arbre quaternaire
        temps_initial = clock();
        R = reconstitueReseauHachage(C3, i);
        temps_final = clock();
        double temps_cpu3 = ((double)(temps_final - temps_initial)) / CLOCKS_PER_SEC;
        liberer_reseau(R);
        
        
        // ecriture dans fichier
        fprintf(fH1,"%d %lf %lf %lf\n", i, temps_cpu, temps_cpu2, temps_cpu3);
        
    }
    liberer_chaines(C1);
    liberer_chaines(C2);
    liberer_chaines(C3);
    fclose(fH1);
    
    //      AVEC CHAINE ALEATOIRE 

    FILE * fH2 = fopen("temps_hachage2.txt","w");
    fprintf(fH2, "taille nbpoints tempsH\n");
    Chaines * C = NULL;
    for (int i=500; i<=50000; i+=5000){

        for (int j=500; j<=5000; j+=500){
            C = generationAleatoire(j, NB_POINTS_CHAINE, X_MAX, Y_MAX);

            temps_initial = clock();
            R = reconstitueReseauHachage(C, i);
            temps_final = clock();
            double temps_cpu = ((double)(temps_final - temps_initial)) / CLOCKS_PER_SEC;
            liberer_reseau(R);
            
            // ecriture dans fichier
            fprintf(fH2,"%d %d %lf \n", i,j, temps_cpu);
            liberer_chaines(C);
        }
        
    }
    fclose(fH2);
    return 0;
}