#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "ShiftBitAPI.c"

int BUFFER_SIZE = 5;

void testUpdateEmptyEXB();
void testUpdateFullEXB();
void testUpdateAlreadyOneElement();

int main(int argc, char** argv) 
{
    //------- EXB of Host -----------//
    testUpdateEmptyEXB();

    testUpdateFullEXB();

    testUpdateAlreadyOneElement();
}

void testUpdateEmptyEXB()
{
    //PacketInEXBHost[i][0] = -1;
    //PacketInEXBHost[i][2] = -1;
    int result = checkUpdateEXBHost(-1, -1, BUFFER_SIZE);
    int isEmptyEXB = result & 1;
    int indexOfUpdate = result >> 1;

    if(indexOfUpdate != 0) //> BUFFER_SIZE)
    {
        printf("Wrong index of Update, should be 0 instead of %d\n", indexOfUpdate);
    }
    if(isEmptyEXB != 1)
    {
        printf("It should be an empty EXB\n");
    }
}

void testUpdateFullEXB()
{
    //PacketInEXBHost[i][0] = 7;
    //PacketInEXBHost[i][2] = 11;
    int result = checkUpdateEXBHost(7, 7 + BUFFER_SIZE - 1, BUFFER_SIZE);
    int isEmptyEXB = result & 1;
    int indexOfUpdate = result >> 1;

    if(indexOfUpdate != 0)//BUFFER_SIZE)
    {
        printf("At testUpdateFullEXB, wrong index of Update, should be 0 instead of %d\n", indexOfUpdate);
    }
    if(isEmptyEXB == 0)
    {
        printf("At testUpdateFullEXB, it should be a non empty EXB\n");
    }
}


void testUpdateAlreadyOneElement()
{
    //The case bottomID = -1;
    int result = checkUpdateEXBHost(7, -1, BUFFER_SIZE);
    int isEmptyEXB = result & 1;
    int indexOfUpdate = result >> 1;

    if(indexOfUpdate != 2)// || indexOfUpdate > BUFFER_SIZE)
    {
        printf("At testUpdateAlreadyOneElement, wrong index of update, should be 1 instead of %d\n", indexOfUpdate);
    }

    if(isEmptyEXB == 0)
    {
        printf("At testUpdateAlreadyOneElement, it should be a non empty EXB\n");
    }

    //The case bottomID = topID;
    result = checkUpdateEXBHost(7, 7, BUFFER_SIZE);
    isEmptyEXB = result & 1;
    indexOfUpdate = result >> 1;

    if(indexOfUpdate != 1 || indexOfUpdate > BUFFER_SIZE)
    {
        printf("Wrong index of update, should be 1 instead of %d\n", indexOfUpdate);
    }

    if(isEmptyEXB == 0)
    {
        printf("It should be a non empty EXB\n");
    }
}