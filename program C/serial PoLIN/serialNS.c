#include <stdio.h>
#include "DataStructures.c"

void display(Packet *n, int length);
//void assignAdj(int n[][]); //, int width, int height);
void assignAdj(int **n, int width, int height);
void echo(int **n, int w, int h);

int main(int argc, char** argv) 
{
    int numOfPorts = 4;
    int numOfSwitches = 20;
    int numOfHosts = 16;
    int numOfInforAboutSwitch = 12;
    //int switches[numOfSwitches][numOfPorts];
    int **switches = NULL;
    switches = malloc( sizeof * switches * numOfSwitches );
    for (int i = 0; i < numOfSwitches; i++ )
    {
      switches[i] = malloc( sizeof *switches[i] * numOfPorts );
    }
    int buffers[numOfSwitches * numOfPorts][numOfInforAboutSwitch];
    Packet hosts[numOfHosts];

    assignAdj(switches, 10, 11);

    for(int i = 0; i < numOfHosts; i++)
    {   
        if(i == 32 - numOfSwitches)
            hosts[i] = createPacket(i + numOfSwitches, i + numOfSwitches, 21, 0);
        else
            hosts[i] = //i + numOfSwitches;  
                    createPacket(i + numOfSwitches, -1, -1, 0);
    }

    //display(hosts, numOfHosts);

    echo(switches, numOfPorts, numOfSwitches);
    //printf("%d", switches[0][1]);

    printf("\nHello World\n");
    return 0;
}

void echo(int **n, int w, int h)
{
    int i, j;
    for(i = 0; i < h; i++) 
    {
        for(j = 0; j < w; j++)
            printf("\tFrom %d to %d\n", i, n[i][j]);
        printf("------------\n");
    }
}

void display(Packet *n, int length)
{
    int i;
    for(i=0; i<length; i++) 
    {
        if(n[i] -> dst != -1)
            printf("\tFrom %d to %d\n", n[i]->id, n[i] -> dst);
    }
}


void assignAdj(int **n/*[][])*/, int width, int height)
{
    /*n[0] = 20;  n[1] = 17; n[2] = 1; n[3] = 2;
    n[4] = 21;  n[5] = 0; n[6] = 18; n[7] = 14;

    n[8] = 22;  n[9] = 8; n[10] = 0; n[11] = 15;
    n[12] = 23;  n[13] = 15; n[14] = 5; n[15] = 8;

    n[16] = 24;  n[17] = 17; n[18] = 18; n[19] = 19;
    n[20] = 25;  n[21] = 3; n[22] = 6; n[23] = 7;

    n[24] = 26;  n[25] = 5; n[26] = 7; n[27] = 18;
    n[28] = 27;  n[29] = 6; n[30] = 5; n[31] = 12;

    n[32] = 28;  n[33] = 2; n[34] = 3; n[35] = 15;
    n[36] = 29;  n[37] = 12; n[38] = 10; n[39] = 16;

    n[40] = 30;  n[41] = 9; n[42] = 19; n[43] = 17;
    n[44] = 31;  n[45] = 13; n[46] = 12; n[47] = 16;

    n[48] = 32;  n[49] = 9; n[50] = 11; n[51] = 7;
    n[52] = 31;  n[53] = 11; n[54] = 16; n[55] = 14;

    n[56] = 34;  n[57] = 13; n[58] = 1; n[59] = 19;
    n[60] = 35;  n[61] = 3; n[62] = 8; n[63] = 2;

    n[64] = 13;  n[65] = 9; n[66] = 11; n[67] = 18;
    n[68] = 0;  n[69] = 4; n[70] = 19; n[71] = 10;

    n[72] = 1;  n[73] = 4; n[74] = 6; n[75] = 16;
    n[76] = 10;  n[77] = 17; n[78] = 4; n[79] = 14;*/

    n[0][0] = 20;  n[0][1] = 17; n[0][2] = 1; n[0][3] = 2;
    n[1][0] = 21;  n[1][1] = 0; n[1][2] = 18; n[1][3] = 14;

    n[2][0] = 22;  n[2][1] = 8; n[2][2] = 0; n[2][3] = 15;
    n[3][0] = 23;  n[3][1] = 15; n[3][2] = 5; n[3][3] = 8;

    n[4][0] = 24;  n[4][1] = 17; n[4][2] = 18; n[4][3] = 19;
    n[5][0] = 25;  n[5][1] = 3; n[5][2] = 6; n[5][3] = 7;

    n[6][0] = 26;  n[6][1] = 5; n[6][2] = 7; n[6][3] = 18;
    n[7][0] = 27;  n[7][1] = 6; n[7][2] = 5; n[7][3] = 12;

    n[8][0] = 28;  n[8][1] = 2; n[8][2] = 3; n[8][3] = 15;
    n[9][0] = 29;  n[9][1] = 12; n[9][2] = 10; n[9][3] = 16;

    n[10][0] = 30;  n[10][1] = 9; n[10][2] = 19; n[10][3] = 17;
    n[11][0] = 31;  n[11][1] = 13; n[11][2] = 12; n[11][3] = 16;

    n[12][0] = 32;  n[12][1] = 9; n[12][2] = 11; n[12][3] = 7;
    n[13][0] = 31;  n[13][1] = 11; n[13][2] = 16; n[13][3] = 14;

    n[14][0] = 34;  n[14][1] = 13; n[14][2] = 1; n[14][3] = 19;
    n[15][0] = 35;  n[15][1] = 3; n[15][2] = 8; n[15][3] = 2;

    n[16][0] = 13;  n[16][1] = 9; n[16][2] = 11; n[16][3] = 18;
    n[17][0] = 0;  n[17][1] = 4; n[17][2] = 19; n[17][3] = 10;

    n[18][0] = 1;  n[18][1] = 4; n[18][2] = 6; n[18][3] = 16;
    n[19][0] = 10;  n[19][1] = 17; n[19][2] = 4; n[19][3] = 14;
}