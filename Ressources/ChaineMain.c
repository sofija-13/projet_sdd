#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Chaine.h"

int main(){
    FILE *f = fopen("./00014_burma.cha", "r");
    Chaines* test = lectureChaines(f);
    fclose(f);

    printf("%d\n", test!=NULL);

    return 0;
}