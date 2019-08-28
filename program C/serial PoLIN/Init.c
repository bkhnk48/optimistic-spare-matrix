#include <stdio.h>
#include "Switches.c"

void display(Packet *n, int length);
void assignAdj(int **n, int width, int height);

void assignAdjant(Switch *switches, Host *hosts, int **n, int h, int w);

void echo(int **n, int w, int h);

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

void InitIntegratedPorts(IntegratedPort **n, int w, int h)
{
    int i, j, k;
    for(i = 0; i < h; i++) 
    {
        for(j = 0; j < w; j++){
            
            printf("\tIntegratedPorts %d of Switch %d has inputPort/outputPort: \n", j, i);
            printf("\t\t Input Port: ");

            IntegratedPort p = (IntegratedPort)malloc(10*sizeof(Packet));

            n[i][j] = p;
            if(n[i][j] == NULL){
                printf("n[i][j] == NULL");
            }

            for(k = 0; k < 5; k++)
            {
                if(n[i][j]->inputPort[k] == NULL)
                    printf("NULL ");
            }
            printf("\n");
            printf("\t\t Output Port: ");
            for(k = 0; k < 5; k++)
            {
                if(n[i][j]->outputPort[k] == NULL)
                    printf("NULL ");
            }
            printf("\n");
        }
        printf("------------\n");
    }
}

void display(Packet *n, int length)
{
    int i;
    for(i=0; i<length; i++) 
    {
        if(n[i] != NULL)
        {

            Packet curr = n[i];
            while(curr != NULL)
            {
                printf("\tFrom %d to %d id = %d||||", curr->src, curr -> dst, curr->id);
                curr = curr->next;
            }
        }
        else
        {
            printf("NULL ");
        }
        
    }
}

void assignAdjant(Switch *switches, Host *hosts, int **n, int h, int w)
{
    int i,j, temp, idOfHost, size;
    size = sizeof(int);
    size = size * 8 - 1;
    for(i = 0; i < h; i++)
    {
        for(j = 0; j < w; j++)
        {
            idOfHost = n[i][j];
            temp = idOfHost - h;
            temp = temp >> size;
            //printf("\t%d %d %d \n", n[i][j], h, temp);
            (switches[i]-> integratedPorts[j]) = malloc(sizeof(IntegratedPort));
            switch(temp)
            {
                case 0: //la HOST
                    switches[i]-> host++;
                    hosts[idOfHost - h] -> aSwitch = i;
                    hosts[idOfHost - h] -> outPort = j;
                    hosts[idOfHost - h] -> lastID = 0;
                    (switches[i]-> integratedPorts[j])->destID = idOfHost;
                    break;
                default: //la Switch
                    (switches[i]-> integratedPorts[j])->destID = j;
                    break;
            }
        }
    }
    /* n[0][0] = 20;  n[0][1] = 17; n[0][2] = 1; n[0][3] = 2;
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
    n[19][0] = 10;  n[19][1] = 17; n[19][2] = 4; n[19][3] = 14;*/
}

void assignAdj(int **n, int width, int height)
{
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