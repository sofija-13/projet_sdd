#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "Hachage.h"

int clef(double x, double y){ 
    return y + (x + y)*(x + y + 1)/2;
}

int main(){
    for (int x=1; x<11; x++){
        for (int y=1; y<11; y++){
            printf("clef(%d,%d)=%d\n", x, y, clef(x, y));
        }
    }
    
    return 0;
}