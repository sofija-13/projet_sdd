#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "Reseau.h"
#include "SVGwriter.h"

Noeud* rechercheCreeNoeudListe(Reseau *R, double x, double y){
    CellNoeud *temp = R->noeuds;
    //recherche noeud de coordonnees (x,y)
    while(temp){
        if (temp->nd->x==x && temp->nd->y==y){
            return temp->nd;
        }
        temp = temp->suiv;
    }
    
    //creation nouveau CellNoeud car absent
    CellNoeud *nouveau = (CellNoeud*)malloc(sizeof(CellNoeud));
    nouveau->nd = (Noeud*)malloc(sizeof(Noeud));
    nouveau->nd->num = (R->nbNoeuds)+1;
    nouveau->nd->x = x;
    nouveau->nd->y = y;
    nouveau->nd->voisins = NULL;

    //ajout en tete de R->noeuds
    nouveau->suiv = R->noeuds;
    R->noeuds = nouveau;
    R->nbNoeuds++;

    return nouveau->nd;
}

Reseau* reconstitueReseauListe(Chaines *C){
    if (C==NULL){ // test validite des argument
        printf("Erreur reconstitueReseauListe : argument NULL\n");
        return NULL;
    }

    Reseau* res = (Reseau*)malloc(sizeof(Reseau));
    res->nbNoeuds = 0;
    res->gamma = C->gamma;
    res->noeuds = NULL;
    res->commodites = NULL;
    
    CellChaine *temp = C->chaines;
    //on parcourt une a une chaque chaine
    while (temp){
        //pour chaque point de la chaine
        CellPoint *tempP = temp->points;
        while (tempP){
            // on teste si le point n’a pas deja ete rencontre auparavant
                // on ajoute dans V un noeud correspondant au point p
                rechercheCreeNoeudListe(res, tempP->x, tempP->y);
            // on met a jour la liste des voisins de p et celles de ses voisins
            // ??
            tempP = tempP->suiv;
        }
        // on conserve la commodite de la chaine
        // ??
        temp = temp->suiv;
    }
    
        

    return res;
} // A TERMINER ---

int nbLiaisons(Reseau *R){
    int res = 0;
    CellCommodite *temp = R->commodites;
    while (temp){
        res++;
        temp = temp->suiv;
    }
    return res;
} // A CORRIGER -----
 
int nbCommodites(Reseau *R){
    CellCommodite* temp = R->commodites;
    int res=0;
    while(temp){
        res++;
        temp=temp->suiv;
    }
    return res;
}

ListeCouple ajouter_couple(ListeCouple l, int a, int b){

}

int dejaVu(ListeCouple l, int a, int b);


void ecrireReseau(Reseau *R, FILE *f){
    // 4 premieres lignes du fichier
    fprintf(f, "NbNoeuds: %d\nNbLiaisons: %d\nNCommodites: %d\nGamma: %d\n\n", R->nbNoeuds, nbLiaisons(R), nbCommodites(R), R->gamma);

    // noeuds
    CellNoeud *temp_noeuds = R->noeuds;  
    while(temp_noeuds){
        fprintf(f, "v %d %lf %lf\n", temp_noeuds->nd->num, temp_noeuds->nd->x, temp_noeuds->nd->y);
        temp_noeuds = temp_noeuds->suiv;
    }
    fprintf(f, "\n");

    // liaisons
    ListeCouple liste = NULL;
    CellNoeud* temp_l = R->noeuds;  
    while(temp_l){
        while (temp_l->nd->voisins){
            if (dejaVu(liste, temp_l->nd->num, temp_l->nd->voisins->suiv->num) == 1){
                // ecrire liaison dans le fichier
                fprintf(f, "l %d %d\n", temp_l->nd->num, temp_l->nd->voisins->suiv->nd->num);
                // ajouter le couple dans la liste des liaisons deja vues
                liste = ajouter_couple(liste, temp_l->nd->num, temp_l->nd->voisins->suiv->nd->num);
            }
            temp_l->nd->voisins = temp_l->nd->voisins->suiv;
        }
        temp_l = temp_l->suiv;
    }
    fprintf(f, "\n");

    // commodites
    CellCommodite *temp_commodites = R->commodites;  
    while(temp_commodites){
        fprintf(f, "k %d %d\n", temp_commodites->extrA->num, temp_commodites->extrB->num);
        temp_commodites = temp_commodites->suiv;
    }

} // A TERMINER ---

void afficheReseauSVG(Reseau *R, char* nomInstance){
    CellNoeud *courN,*courv;
    SVGwriter svg;
    double maxx=0,maxy=0,minx=1e6,miny=1e6;

    courN=R->noeuds;
    while (courN!=NULL){
        if (maxx<courN->nd->x) maxx=courN->nd->x;
        if (maxy<courN->nd->y) maxy=courN->nd->y;
        if (minx>courN->nd->x) minx=courN->nd->x;
        if (miny>courN->nd->y) miny=courN->nd->y;
        courN=courN->suiv;
    }
    SVGinit(&svg,nomInstance,500,500);
    courN=R->noeuds;
    while (courN!=NULL){
        SVGpoint(&svg,500*(courN->nd->x-minx)/(maxx-minx),500*(courN->nd->y-miny)/(maxy-miny));
        courv=courN->nd->voisins;
        while (courv!=NULL){
            if (courv->nd->num<courN->nd->num)
                SVGline(&svg,500*(courv->nd->x-minx)/(maxx-minx),500*(courv->nd->y-miny)/(maxy-miny),500*(courN->nd->x-minx)/(maxx-minx),500*(courN->nd->y-miny)/(maxy-miny));
            courv=courv->suiv;
        }
        courN=courN->suiv;
    }
    SVGfinalize(&svg);
}

/*
void liberer_reseau(Reseau *R){
    while(R->noeuds){

        CellNoeud *temp_noeuds = R->noeuds;
        R->noeuds = R->noeuds->suiv;
        
        while(temp_noeuds){
            CellNoeud *temp_voisins = temp_noeuds->voisins;
            temp_noeuds = temp_noeuds->suiv;
        }

        free(temp_noeuds);
        
       while (temp_noeuds) {
            CellNoeud *temp_voisin = temp_noeuds;
            temp_noeuds = temp_noeuds->suiv;
            free(temp_voisin);
        }
    }
    while(R->commodites){
        CellCommodite *temp_commodites = R->commodites;
        R->commodites = R->commodites->suiv;
        // free extrA
        while(temp_commodites->extrA->voisins){
            CellNoeud* tempA = temp_commodites->extrA->voisins;
            temp_commodites->extrA->voisins = temp_commodites->extrA->voisins->suiv;
            free()
        }
        free(temp_commodites->extrA);
        // free extrB
        while(temp_commodites->extrB->voisins){
            CellNoeud* tempB = temp_commodites->extrB->voisins;
            temp_commodites->extrB->voisins = temp_commodites->extrB->voisins->suiv;
        }
        free(temp_commodites->extrB);

        free(temp_commodites);
    }
    free(R->commodites);

    free(R);
}
*/