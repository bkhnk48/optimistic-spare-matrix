#include<stdio.h>
#include<stdlib.h>
#include "Simulation.h"
#include "RoutingPath.h"
#include <limits.h>

int executeEventC(int* avail, int* timeOfB, int* credit, int *outport, int *dstIDs, int* timeOfC, int curr, int hostID
                        , int BUFFER_SIZE, int path, int *Links
                    )
{
    /*if( Links[idOfLink][2] == -1)//KHONG co goi tin tren duong truyen
        {
            //Link[index][0]: id cua nut nguon
            //Link[index][1]: id cua nut dich
            //Link[index][2]: id cua goi tin
            //Link[index][3] = -1;//id cua host nguon
            //Link[index][4] = -1;//id cua host dich
            //Link[index][5] = 0;//nix-vector
            //Link[index][6] = 0;//kich thuoc cua goi tin
            //Link[index][7] = 0;//thoi diem bit dau den
            //Link[index][8] = 0;//thoi diem bit cuoi den
            //Link[index][9] = 0;//bandwidth
            //Link[index][10] = 0;//bang 0 nghia la nut nguon ko phai la Host
            //Link[index][11] = 0;//bang 1 nghia la nut dich la Host
            //Link[index][12] = 0;//Hop count

            //int **outport; //an array with size = 2 x BUFFER_SIZE
            //outport[0][i]: all ids of packets in outport
            //outport[1][i]: all destinations of packets in outport*/
    
    int alpha = *avail;
    alpha = (alpha)>>(8*sizeof(int) - 1);
    int beta = *timeOfC;
    beta = (beta - curr) >> (8*sizeof(int) - 1);
    int gamma = *timeOfC;
    gamma = (curr - gamma) >> (8*sizeof(int) - 1);
    int temp, t;
    int hasANewEventB = 0;
    //if(*avail > 0//Neu co goi tin o top cua outport
    //    && *timeOfC == curr //va goi tin o vi tri top nay co nhu cau duoc gui di
    //    )
    switch(alpha + beta + gamma)
    {
        case 0:
            t = *credit;
            t = t>>(8*sizeof(int) - 1);
            //if(*credit > 0)//neu inport cua switch tiep theo con cho trong
            switch(t)
            {
                case 0:
                    //tao su kien tren link
                    Links/*[idOfLink]*/[2] = outport[0];//gan id cua goi tin vao Link
                    Links/*[idOfLink]*/[3] = hostID;//id cua host nguon
                    Links/*[idOfLink]*/[4] = dstIDs[0];//id cua host dich
                    Links/*[idOfLink]*/[5] = path;//nix vector
                    Links/*[idOfLink]*/[6] = (int)1e5;
                    Links/*[idOfLink]*/
                            [7] = (int)(1e9 * Links/*[idOfLink]*/
                                                [6] / Links/*[idOfLink]*/
                                                            [9]);
                    Links/*[idOfLink]*/[8] = 1001;
                    Links/*[idOfLink]*/[12] = 0;
                    *timeOfC = -1;//xoa di thoi diem thuc thi su kien (C)
                    temp = *credit;
                    *credit = temp - 1;//thay doi gia tri credit
                    int j;
                    for(j = 0; j < BUFFER_SIZE - 1; j++)
                    {
                        outport[j] = outport[j+1];
                        dstIDs[j] = outport[j+1];
                    }
                    outport[BUFFER_SIZE - 1] = -1;
                    dstIDs[BUFFER_SIZE - 1] = -1;
                    temp = *avail;
                    *avail = temp - 1;
                    temp = *avail;
                    temp = (temp - BUFFER_SIZE)>>(8*sizeof(int)- 1);
                    //if(*avail < BUFFER_SIZE)
                    t = *timeOfB;
                    //{
                    *timeOfB = -temp*curr + (1 + temp)*t;  
                    hasANewEventB = -temp;
                    //}
                    *timeOfC = -1;
                break;
            }
        break;
    }
    return hasANewEventB;
    
}

void run(Graph g, RAlgorithm ra, int *path, int stop, int curr)
{
    /*
    This data structure respresents the host
    struct HOST{
        int BUFFER_SIZE: BUFFER_SIZE
        int linkID: ID of link from host itself
        int hostID: id cua host (trong danh sach cac nut cua toan mang)

        int *allEvents;
                    //allEvents[0]: credit count
                    //allEvents[1]: id of the sent packet
                    //allEvents[2]: id of received packet in next switch
                    //allEvents[3]: time to execute event (I)
                    //allEvents[4]: time to execute event (B)
                    //allEvents[5]: time to execute event (C)
                    //allEvents[6]: num of packets in outport
        int *outport; //an array with size = BUFFER_SIZE
                    //outport: all ids of packets in outport
        int *dstIDs; //dstIDs: all destinations of packets in outport
    };
    */

    int i, j;
    Host *Hosts = g->Hosts;
    //int **Hosts;
    int **Links = g->Links;
    int *SwitchIndexes = g->SwitchIndexes;
    int numOfHosts = g->numOfHosts;
    int BUFFER_SIZE = g->BUFFER_SIZE;
    int CYCLE_PACKET = g->CYCLE_PACKET;
    int dst = path[path[0] - 1];
    int RETRY_TIME = 3;
    Queue **queues = g->queues;
    int minNextTime = INT_MAX;
    int sizeOfShift = (8*sizeof(int)- 1);
    
    for(i = 0; i < numOfHosts; i++)
    {
        int hostID = Hosts[i]->hostID;
        int idOfLink = Hosts[i]->linkID;
        int *allEvents = Hosts[i]->allEvents;
        int credit = allEvents[0];
        int sentID = allEvents[1];
        int receivedID = allEvents[2];
        int timeOfI = allEvents[3];
        int timeOfB = allEvents[4];
        int timeOfC = allEvents[5];
        int avail = allEvents[6];
        Queue *q = queues[i];
        int *outport = Hosts[i]->outport;
        int *dstIDs = Hosts[i]->dstIDs;
        int nextCycle = INT_MAX;
        int hasNewEventB = 0;

        Queue t = NULL;

        //execute event A:
        switch(curr % CYCLE_PACKET)
        {
            case 0: //if this is the time to generate packet
                nextCycle = curr + CYCLE_PACKET;
                switch (q[0]->id)
                {
                    case -1:
                        q[0]->id = curr / CYCLE_PACKET;//curr/CYCLE la id cua goi tin
                        q[0]->hostID = hostID;
                        q[0]->dstID = dst;
                        timeOfB = curr;//create event B
                        break;
                
                    default:
                        switch (q[1]->id)
                        {
                            case -1:
                                q[1]->id = curr / CYCLE_PACKET;//curr/CYCLE la id cua goi tin
                                q[1]->hostID = hostID;
                                q[1]->dstID = dst;
                                q[1]->next = NULL;
                                break;
                        
                            default:
                                t = malloc(sizeof(Queue));
                                t->id = curr / CYCLE_PACKET;//curr/CYCLE la id cua goi tin
                                t->hostID = hostID;
                                t->dstID = dst;
                                t->next = NULL;
                                q[1]->next = t;
                                q[1] = t;
                                break;
                        }
                        break;
                }
                break;
        }
        
        //endof executing event A:

        //execute event I
        int alpha = (curr - timeOfI) >> (8*sizeof(int)-1);//bang 0 hoac -1
        int beta = (timeOfI - curr) >> (8*sizeof(int)-1);//bang 0 hoac -1
        timeOfI = (1 + alpha + beta)*(-1) - (alpha + beta)*timeOfI;

        int gamma = (sentID - receivedID) >> (8*sizeof(int)-1);//bang 0 hoac -1
        int delta = (receivedID - sentID) >> (8*sizeof(int)-1);//bang 0 hoac -1
        int espilon = sentID >> (8*sizeof(int)-1);//bang 0 hoac -1
        timeOfC = (1 + espilon)*(1 + gamma + delta)*curr - ((espilon + gamma + delta)%2)*timeOfC;
        credit += (1 + espilon)*(1 + gamma + delta);
        //endof executing event I
        

        //execute event C
        //NOTE: THIS CODE HAS A CLONE which begins at line 229
        switch( Links[idOfLink]
                [2])//KHONG co goi tin tren duong truyen
        {
            case -1:
                hasNewEventB = executeEventC(&avail, &timeOfB, &credit, outport, dstIDs, &timeOfC, curr, 
                            hostID, BUFFER_SIZE, path[1], 
                            Links[idOfLink]
                );
                break;
        }
        
        //execute event B             
        switch(timeOfB - curr)
        {
            case 0:
                alpha = (avail - BUFFER_SIZE) >> (8*sizeof(int)-1); //(avail < BUFFER_SIZE)//if there is a empty slot
                switch (alpha)
                {
                    case -1:
                        outport[avail] = q[0]->id;//push the packet in the top source queue to this slot
                        dstIDs[avail] = q[0]->dstID;
                        avail++;
                        Queue next = q[0]->next;
                        switch(next->id - q[1]->id)//if there is zero to one remaining packet in source queue
                        {    
                            case 0:
                                q[0] = q[0]->next;
                                Queue t = NULL;
                                t = malloc(sizeof(Queue));
                                t->id = -1;
                                t->dstID = -1;
                                t->hostID = -1;
                                t->next = 0;
                                q[1] = t;
                                break;
                            default:
                                q[0] = q[0]->next;
                                break;
                        }
                        //if(avail == 1)//neu goi tin moi vao outport cung la goi tin duy nhat
                        beta = (avail - 1)>>(8*sizeof(int) - 1);
                        gamma = (1 - avail)>>(8*sizeof(int) - 1);
                        timeOfC = (1 + beta + gamma) * curr - (beta + gamma)*timeOfC;
                        timeOfB = -1;
                        break;
                
                    default: 
                        timeOfB = curr + RETRY_TIME;//Khong co outport nao trong, danh phai doi.
                        break;
                }
            break;
        }
        //endof executing event B

        //execute event C
        //NOTE: THIS CODE HAS A CLONE which begins at line 179
        switch( Links[idOfLink]
                [2])//KHONG co goi tin tren duong truyen
        {
            case -1:
                hasNewEventB = executeEventC(&avail, &timeOfB, &credit, outport, dstIDs, &timeOfC, curr, 
                            hostID, BUFFER_SIZE, path[1], 
                            Links[idOfLink]
                );

                int itemInQueue = (q[0]->id) >> sizeOfShift;
                timeOfB = hasNewEventB * (1 + itemInQueue)*curr - (1 - hasNewEventB)*itemInQueue*timeOfB;
                break;
        }
        int x = timeOfI, y = timeOfB, z = timeOfC;
        
        int check = x >> sizeOfShift;
        x = (1 + check)*x - check*INT_MAX;
        check = y >> sizeOfShift;
        y = (1 + check)*y - check*INT_MAX;
        check = z >> sizeOfShift;
        z = (1 + check)*z - check*INT_MAX;

        check = x - y;
        check = check >> sizeOfShift;//check = -1 nghia la x < y, 0 nghia la x >= y
        x = (1 + check)*y -check*x;
        check = x - z;
        check = check >> sizeOfShift;//check = -1 nghia la x < z, 0 nghia la x >= z
        x = (1 + check)*z -check*x;

        check = x - nextCycle;
        check = check >> sizeOfShift;//check = -1 nghia la x < z, 0 nghia la x >= z
        x = (1 + check)*nextCycle -check*x;

        check = minNextTime - x;
        check = check >> sizeOfShift;//check = -1 nghia la minNextTime < x, 0 nghia la minNextTime >= x
        minNextTime = (1 + check)*x - check*minNextTime; 

        allEvents[0] = credit;
        allEvents[3] = timeOfI;
        allEvents[4] = timeOfB;
        allEvents[5] = timeOfC;
        allEvents[6] = avail;
    }
    curr++;
}