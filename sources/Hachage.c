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
} // c'est la meme que celle du projet precedent
// attendre correction mini-projet pour la valider

/*
Noeud* rechercheCreeNoeudHachage(Reseau* R, TableHachage* H, double x, double y){
        
}

Reseau* reconstitueReseauHachage(Chaines *C, int M){
}
*/

/*
int main(){
    for (int x=1; x<11; x++){
        for (int y=1; y<11; y++){
            printf("clef(%d,%d)=%d\n", x, y, clef(x, y));
        }
    }
    
    return 0;
}
*/