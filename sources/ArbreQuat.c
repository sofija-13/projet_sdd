#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "ArbreQuat.h"
#include "SVGwriter.h"

void chaineCoordMinMax(Chaines* C, double* xmin, double* ymin, double* xmax, double* ymax){
    if(C == NULL){
        printf("Chaine Vide \n");
        return;
    }

    CellChaine* temp = C->chaines;
    CellPoint* premier = C->chaines->points;
    double Xmin = premier->x;
    double Ymin = premier->y;
    double Xmax = premier->x;
    double Ymax = premier->y;
    while(temp){
        CellPoint* p = temp->points;
        while(p){
            //si coordonnées plus petit que Xmin et Ymin
            if (p->x < Xmin && p->y < Ymin){
                Xmin = p->x;
                Ymin = p->y;
            }
            //si coordonnées plus grand que Xmax et Ymax
            else if (p->x > Xmax && p->y > Ymax){
                Xmax = p->x;
                Ymax = p->y;
            }
            p = p->suiv;
        }
        temp = temp->suiv;
    }

}