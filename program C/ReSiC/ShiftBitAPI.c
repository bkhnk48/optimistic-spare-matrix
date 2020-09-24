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
int checkUpdateEXBHost(int topEXB, int bottomEXB, int topSQ, int BUFFER_SIZE)
{
    //Neu topEXB co id = 1 va topSQ co id = 3
    //Vay thi trong EXB da co 2 pkt voi id = 1 va 2.
    //do EXBHost chua id cua goi tin (tai cac phan tu co chi so chan)
    //con EXBHost chua ca dst cua goi tin (tai cac phan tu co chi so le)
    //nen chi so cua EXBHost can cap nhat ID goi tin se la 2*(3 - 1) = 4;
    int isEmptyEXB = -(topEXB >> 31);//chi nhan gia tri 0 hoac 1.

    int emptySQ = - (topSQ >> 31);
    
    //int indexOfUpdate = (topSQ - topEXB)*(1 - emptySQ);//code nay se danh cho EXB cua switch
    //int indexOfUpdate = (1 - isEmptyEXB)*(1 - emptySQ);
    
    int isFullEXB = bottomEXB - topEXB + 1 - BUFFER_SIZE;
    isFullEXB = 1 + (isFullEXB >> 31); //0 nghia la EXB chua full, 1 nghia la EXB da full.
    
    int allowUpdate = 1 - isFullEXB; //0 nghia la khong cho update, 1 nghia la cho update
    
    int indexOfUpdate = (1 - isEmptyEXB)*(1 - emptySQ)*allowUpdate;

    int result = (indexOfUpdate << 1) + (1 & isFullEXB);
    //int result = indexOfUpdate ; //| isFullEXB;
    return result;
}