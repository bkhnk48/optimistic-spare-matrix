#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>

void passByRefArray(int arr[3][2]);

int main(int argc, char** argv) 
{
    int arr[3][2];
    int i, j;
    for(i = 0; i < 3; i++)
    {
        for(j = 0; j < 2; j++)
        {
            arr[i][j] = 0;
        }
    }

    printf("The test following proves that we can pass array as reference without using ASKTERISK *\n");
    passByRefArray(arr);

    for(i = 0; i < 3; i++)
    {
        for(j = 0; j < 2; j++)
        {
            assert(arr[i][j] == i*j + i + j);
        }
    }

}

void passByRefArray(int arr[3][2])
{
    int i, j;
    for(i = 0; i < 3; i++)
    {
        for(j = 0; j < 2; j++)
        {
            arr[i][j] = i*j + i + j;
        }
    }
}