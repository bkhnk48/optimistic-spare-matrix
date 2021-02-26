#include <stdio.h>
#include <stdlib.h>

void writeTime(unsigned long *time, char *fileName, const unsigned long length){
    FILE *fptr;
    fptr = fopen(fileName,"w");
    unsigned long i = 0;
    for(i = 0; i < length; i++){
        fprintf(fptr,"%ld\n",time[i]);
    }
    fclose(fptr);
}

void readTime(char *file1, char *file2, const unsigned long length){
    FILE *f1, *f2;
    f1 = fopen(file1,"r");
    unsigned long *time1 = malloc(length * sizeof(unsigned long));
    unsigned long *time2 = malloc(length * sizeof(unsigned long));
    unsigned long i = 0;
    for(i = 0; i < length; i++){
        time1[i] = -1;
        time2[i] = -1;
    }
    for(i = 0; i < length; i++){
        //fprintf(f1,"%ld\n",time[i]);
    }
    fclose(f1);
    fclose(f2);
}