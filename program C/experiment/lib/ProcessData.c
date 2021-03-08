#include <stdio.h>
#include <stdlib.h>
#include "Node.c"

void writeTime(Node *Events, char *fileName){
    FILE *fptr;
    fptr = fopen(fileName,"w");
    unsigned long i = 0;
    Node *temp = Events;
    while(temp != NULL){
        fprintf(fptr,"%ld\n", temp->endTime);
        temp = temp->next;
        i++;
    }
    fclose(fptr);
}

void writeLine(unsigned long time, FILE *fptr){
    fprintf(fptr,"%ld\n", time);
}

void checkOrder(char *file1, char *file2, const unsigned long length){
    FILE *f1, *f2;
    f1 = fopen(file1,"r");
    f2 = fopen(file2,"r");
    unsigned long *time1 = malloc(length * sizeof(unsigned long));
    //unsigned long *time2 = malloc(length * sizeof(unsigned long));
    unsigned long i = 0;
    for(i = 0; i < length; i++){
        time1[i] = -1;
        //time2[i] = -1;
    }
    unsigned long x;
    i = 0;
    
    char line[256];

    while (fgets(line, sizeof(line), f1) && i < length) {
        time1[i] = atol(line);
        i++;
    }
    fclose(f1);
    i = 0; 
    unsigned long j = 0;
    while (fgets(line, sizeof(line), f2) && i < length) {
        x = atol(line);
        if(x != time1[i]){
            printf("Difference is %ld at i = %ld\n", x, i);
            break;
        }
        i++;
    }
    fclose(f2);
}

void readTime(char *file1, char *file2, const unsigned long length){
    FILE *f1, *f2;
    f1 = fopen(file1,"r");
    f2 = fopen(file2,"r");
    unsigned long *time1 = malloc(length * sizeof(unsigned long));
    //unsigned long *time2 = malloc(length * sizeof(unsigned long));
    unsigned long i = 0;
    for(i = 0; i < length; i++){
        time1[i] = -1;
        //time2[i] = -1;
    }
    unsigned long x;
    i = 0;
    
    char line[256];

    while (fgets(line, sizeof(line), f1) && i < length) {
        time1[i] = atol(line);
        i++;
    }
    fclose(f1);
    i = 0; int c = 0;
    unsigned long j = 0;
    while (fgets(line, sizeof(line), f2) && i < length) {
        x = atol(line);
        if(c < 3){
            j = 0;
            for(j = 0; j < length; j++){
                if(x == time1[j])
                    break;
            }
            if(j == length){
                printf("Difference is %ld at i = %ld\n", x, i);
                c++;
            }
        }
        i++;
    }
    fclose(f2);
}