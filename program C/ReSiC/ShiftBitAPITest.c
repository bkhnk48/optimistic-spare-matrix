#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "ShiftBitAPI.c"

int BUFFER_SIZE = 5;

void testUpdateEmptyEXB();
void testUpdateFullEXB();
void testUpdateWithEmptySQ();
void testUpdateWithNormalSQEXB();

int main(int argc, char** argv) 
{
    //------- EXB of Host -----------//
    testUpdateEmptyEXB();

    testUpdateFullEXB();

    testUpdateWithEmptySQ();

    testUpdateWithNormalSQEXB();
}

void testUpdateEmptyEXB()
{
    //PacketInEXBHost[i][0] = -1;
    //PacketInEXBHost[i][2] = -1;
    int result = checkUpdateEXBHost(-1, -1, -1, BUFFER_SIZE);
    int isFullEXB = result & 1;
    int indexOfUpdate = result >> 1;

    if(indexOfUpdate != 0 || indexOfUpdate > BUFFER_SIZE)
    {
        printf("Wrong index of Update, should be 0 instead of %d\n", indexOfUpdate);
    }
    if(isFullEXB == 1)
    {
        printf("It should be an empty EXB\n");
    }
}

void testUpdateFullEXB()
{
    //PacketInEXBHost[i][0] = 7;
    //PacketInSQ[i][1] = 7 + BUFFER_SIZE;
    int result = checkUpdateEXBHost(7, 7 + BUFFER_SIZE - 1, 7 + BUFFER_SIZE, BUFFER_SIZE);
    int isFullEXB = result & 1;
    int indexOfUpdate = result >> 1;

    if(indexOfUpdate != 0 || indexOfUpdate > BUFFER_SIZE)
    {
        printf("At testUpdateFullEXB, wrong index of Update, should be 0 instead of %d\n", indexOfUpdate);
    }
    if(isFullEXB == 0)
    {
        printf("At testUpdateFullEXB, it should be a full EXB\n");
    }
}


void testUpdateWithEmptySQ()
{
    //The case SQ is empty;
    int i = 0;
    for(i = 0; i < BUFFER_SIZE; i++)
    {
        int result = checkUpdateEXBHost(7, 7 + i, -1, BUFFER_SIZE);
        int isFullEXB = result & 1;
        int indexOfUpdate = result >> 1;

        if(indexOfUpdate != 0 || indexOfUpdate > BUFFER_SIZE)
        {
            printf("At testUpdateWithEmptySQ, wrong index of update, should be 0 instead of %d\n", indexOfUpdate);
        }

        if(isFullEXB == 1 && i == 0)
        {
            printf("At testUpdateWithEmptySQ, it should NOT be a full EXB\n");
        }

        if(isFullEXB == 1 && i < BUFFER_SIZE - 1 && i > 0)
        {
            printf("At testUpdateWithEmptySQ, it should NOT be a full EXB\n");
        }

        if(isFullEXB == 0 && i == BUFFER_SIZE - 1)
        {
            printf("At testUpdateWithEmptySQ, it should be a full EXB\n");
        }
    }

    return;
}

void testUpdateWithNormalSQEXB()
{
    int i = 0;
    for(i = 1; i < BUFFER_SIZE; i++)
    {
        int result = checkUpdateEXBHost(7, i + 7, i + 7 + 1, BUFFER_SIZE);
        int isFullEXB = result & 1;
        int indexOfUpdate = result >> 1;
        if(indexOfUpdate != ((i + 1) % BUFFER_SIZE) || indexOfUpdate > BUFFER_SIZE)
        {
            printf("At testUpdateWithNormalSQEXB, wrong index of update, should be %d instead of %d\n", 
                        (i + 1) % BUFFER_SIZE, indexOfUpdate);
        }

        if(isFullEXB == 1 && i < BUFFER_SIZE - 1)
        {
            printf("At testUpdateWithNormalSQEXB, it should be a non full EXB\n");
        }
    }
}