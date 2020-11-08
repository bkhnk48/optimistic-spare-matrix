#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int my_random(int minN, int maxN){
    return minN + rand() % (maxN + 1 - minN);
}



int main(){
    srand((int)time(0));
    FILE *fptr;
    fptr = fopen("random1000.txt","w");
    int r;
    for(int i = 0; i < 1000; ++i){
        r = my_random(1,100);
        if(r <= 21) r = 0;
        fprintf(fptr,"%d\n", r);

    }
    fclose(fptr);

    return 0;
}