#include <stdio.h> 
#include <stdlib.h> 
#include <limits.h> 
#include "ErrorReport.c"

int assignLink(int *Link, int width, int height, int bandwidth);


void echo(int *Link, int count);

void echo(int *Link, int count)
{
    int i = 0, j; int index = 0;
    while(i < count)
    {
        printf("\tFrom %d to %d at Link[%d]\n", Link[index], Link[index+1], index);
        index += 10;
        //i += 10;
        i++;
    }
}


int assignLink(int *Link, int width, int height, int bandwidth)
{
    int **n = NULL;      n = malloc( sizeof * n * height );
    for (int i = 0; i < height; i++ ){   n[i] = malloc( sizeof *n[i] * width);    }
    n[0][0] = 20;  n[0][1] = 17; n[0][2] = 1; n[0][3] = 2; //4 0->20, 17, 1, 2
    n[1][0] = 21;  n[1][1] = 0; n[1][2] = 18; n[1][3] = 14; //3:  1->21, 18, 14  => 7

    n[2][0] = 22;  n[2][1] = 8; n[2][2] = 0; n[2][3] = 15;//3: 2->22, 8, 15
    n[3][0] = 23;  n[3][1] = 15; n[3][2] = 5; n[3][3] = 8;//4: 3->23, 15, 5, 8  => 7 +7 = 14

    n[4][0] = 24;  n[4][1] = 17; n[4][2] = 18; n[4][3] = 19;//4: 4->24, 17, 18, 19
    n[5][0] = 25;  n[5][1] = 3; n[5][2] = 6; n[5][3] = 7;//3: 5->25, 6, 7 => 14 + 7 =21

    n[6][0] = 26;  n[6][1] = 5; n[6][2] = 7; n[6][3] = 18;//3: 6->26, 7, 18
    n[7][0] = 27;  n[7][1] = 6; n[7][2] = 5; n[7][3] = 12;//2: 7->27, 12  => 21 + 5 = 26

    n[8][0] = 28;  n[8][1] = 2; n[8][2] = 3; n[8][3] = 15;//2: 8->28; 8->15
    n[9][0] = 29;  n[9][1] = 12; n[9][2] = 10; n[9][3] = 16;//4: 9->29, 12, 10, 16  => 26 + 6 = 32

    n[10][0] = 30;  n[10][1] = 9; n[10][2] = 19; n[10][3] = 17;//3: 10->30, 19, 17
    n[11][0] = 31;  n[11][1] = 13; n[11][2] = 12; n[11][3] = 16;//4: 11->31, 13, 12, 16  => 32 + 7 = 39

    n[12][0] = 32;  n[12][1] = 9; n[12][2] = 11; n[12][3] = 7;//1: 12->32
    n[13][0] = 33;  n[13][1] = 11; n[13][2] = 16; n[13][3] = 14;//3: 13->33, 16, 14  => 39 + 4 = 43

    n[14][0] = 34;  n[14][1] = 13; n[14][2] = 1; n[14][3] = 19;//2: 14->34, 19
    n[15][0] = 35;  n[15][1] = 3; n[15][2] = 8; n[15][3] = 2;//1: 15->35 => 43 + 3 = 46

    n[16][0] = 13;  n[16][1] = 9; n[16][2] = 11; n[16][3] = 18;//1: 16->18
    n[17][0] = 0;  n[17][1] = 4; n[17][2] = 19; n[17][3] = 10;//1: 17->19   => 46 + 2 = 48

    n[18][0] = 1;  n[18][1] = 4; n[18][2] = 6; n[18][3] = 16;//0:
    n[19][0] = 10;  n[19][1] = 17; n[19][2] = 4; n[19][3] = 14;//0:

    int i, j;    int count = 0;    int index = 0;

    for(i = 0; i < height; i++)
    {
        for(j = 0; j < width; j++)
        {   
            Link[index] = i; /*id cua nut nguon*/
            Link[index + 1] = n[i][j];/*id cua nut dich*/
            Link[index + 2] = -1;/*id cua goi tin*/
            Link[index + 3] = -1;//id cua host nguon
            Link[index + 4] = -1;//id cua host dich
            Link[index + 5] = 0;//nix-vector
            Link[index + 6] = 0;//kich thuoc goi tin
            Link[index + 7] = 0;//thoi diem bit dau den
            Link[index + 8] = 0;//thoi diem bit cuoi cung den
            Link[index + 9] = bandwidth;//bang thong
            //printf("\t---> From %d to %d at Link[%d]\n", Link[index], Link[index + 1], index);
            index = index + 10;
            
            if(n[i][j] >= height)
            {
                int temp = (n[i][j] - height)*10 + height*width*10;
                
                Link[temp] = n[i][j]; /*id cua nut nguon*/
                Link[temp + 1] = i;/*id cua nut dich*/
                Link[temp + 2] = -1;/*id cua goi tin*/
                Link[temp + 3] = -1;//id cua host nguon
                Link[temp + 4] = -1;//id cua host dich
                Link[temp + 5] = 0;//nix-vector
                Link[temp + 6] = 0;//kich thuoc goi tin
                Link[temp + 7] = 0;//thoi diem bit dau den
                Link[temp + 8] = 0;//thoi diem bit cuoi cung den
                Link[temp + 9] = bandwidth;//bang thong

                //printf("\t====From %d to %d at Link[%d]\n", Link[temp], Link[temp + 1], temp);
            }
            count += 2;
        }
    }
    return count; //Could it return 48?

}