#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Reseau.h"

int main(int argc, char** argv){
    if (argc != 3){
        printf("Usage : %s <nom fichier .cha> <methode a utiliser>\n", argv[0]);
        printf("0\tliste\n");
        printf("1\ttable de hachage\n");
        printf("2\tarbre\n");
        return 1;
    }

    return 0;
}