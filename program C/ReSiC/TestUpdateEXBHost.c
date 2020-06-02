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

void testUpdateEmptyEXB(){}
void testUpdateFullEXB(){}
void testUpdateWithEmptySQ(){}
void testUpdateWithNormalSQEXB(){}


