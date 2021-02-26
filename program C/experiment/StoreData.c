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
    //f2 = fopen(file2,"r");
    unsigned long *time1 = malloc(length * sizeof(unsigned long));
    unsigned long *time2 = malloc(length * sizeof(unsigned long));
    unsigned long i = 0;
    for(i = 0; i < length; i++){
        time1[i] = -1;
        time2[i] = -1;
    }
    unsigned long x;
    i = 0;
    
    char line[256];

    while (fgets(line, sizeof(line), f1) && i < length) {
        //printf("%s", line); 
        time1[i] = atol(line);
        printf("[%ld] = %ld ", i, time1[i]);
        i++;
    }
    fclose(f1);
    //fclose(f2);
}