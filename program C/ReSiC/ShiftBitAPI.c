#include <stdio.h>

/* This function will return 0 if a != b
 * in case a == b the result is 1
 */

int checkEqual(int a, int b)
{
    int x = a - b;
    int y = b - a;
    //x*y = (-1) * ((a-b)^2 <= 0
    x = x*y;
    y = x >> 31;
    y = 1 + y;
    return y;
}

//Ham nay dua tren mot gia thuyet rang:
// kich thuoc cua buffer_size la gioi han
int checkUpdateEXBHost(int topID, int bottomID, int BUFFER_SIZE)
{
    int isEmptyEXB = -(topID >> 31);//chi nhan gia tri 0 hoac 1.
    
    int indexOfUpdate = (bottomID - topID + 1);
    
    int isFullEXB = indexOfUpdate - BUFFER_SIZE;
    isFullEXB = 1 + (isFullEXB >> 31); //0 nghia la EXB chua full, 1 nghia la EXB da full.
    
    int allowUpdate = 1 - isFullEXB; //0 nghia la khong cho update, 1 nghia la cho update
    
    indexOfUpdate = (1 - isEmptyEXB)*indexOfUpdate*allowUpdate;

    indexOfUpdate = indexOfUpdate << 1;
    int result = indexOfUpdate & isEmptyEXB;
    return result;
}