#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>

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

void testUpdateEmptyEXB(){


}
void testUpdateFullEXB(){

}
void testUpdateWithEmptySQ(){
    //EXB co goi tin (chua full) va SQ KHONG co goi tin
    int topEXB = 2;
    int bottomEXB = 5;
    int topSQ = -1;
    int BUFFER_SIZE = 5;
    int result = checkUpdateEXBHost(topEXB, bottomEXB, topSQ, BUFFER_SIZE);
    int isFullEXB = result & 1;
    int indexOfUpdate = result >> 1;
    printf("indexOfUpdate = %d\n", indexOfUpdate);
    assert(indexOfUpdate == 1);
}
void testUpdateWithNormalSQEXB(){
    //EXB co goi tin (chua full) va SQ van co goi tin
    int topEXB = 2;
    int bottomEXB = 5;
    int topSQ = 6;
    int BUFFER_SIZE = 5;
    int result = checkUpdateEXBHost(topEXB, bottomEXB, topSQ, BUFFER_SIZE);
    int isFullEXB = result & 1;
    int indexOfUpdate = result >> 1;
    printf("indexOfUpdate = %d\n", indexOfUpdate);
    assert(indexOfUpdate == 1);
}


