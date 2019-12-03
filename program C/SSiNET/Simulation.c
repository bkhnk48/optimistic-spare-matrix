#include<stdio.h>
#include<stdlib.h>
#include "Simulation.h"
#include "RoutingPath.h"


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

        //execute event A:
        if(curr % CYCLE_PACKET == 0)//if this is the time to generate packet
        {
            if(q[0]->id == -1)//if source queue KHONG con goi tin
            {
                q[0]->id = curr / CYCLE_PACKET;//curr/CYCLE la id cua goi tin
                q[0]->hostID = hostID;
                q[0]->dstID = dst;
                timeOfB = curr;//create event B
            }
            else
            {

                if(q[1]->id == -1)
                {
                    q[1]->id = curr / CYCLE_PACKET;//curr/CYCLE la id cua goi tin
                    q[1]->hostID = hostID;
                    q[1]->dstID = dst;
                    q[1]->next = NULL;
                }
                else{
                    Queue t = NULL;
                    t = malloc(sizeof(Queue));
                    t->id = curr / CYCLE_PACKET;//curr/CYCLE la id cua goi tin
                    t->hostID = hostID;
                    t->dstID = dst;
                    t->next = NULL;
                    q[1]->next = t;
                    q[1] = t;
                }
            }   
        }
        //endof if(curr % CYCLE_PACKET == 0)

        //execute event I
        if(curr == timeOfI
            )
        {
            if(sentID == receivedID && sentID != -1)
            {
                timeOfC = curr;//create event C
                credit++;//thay doi gia tri credit
            }
            timeOfI = -1;
        }

        //execute event C
        //NOTE: THIS CODE HAS A CLONE which begins at line 200
        if( Links[idOfLink][2] == -1)//KHONG co goi tin tren duong truyen
        {
            //Link[index][0]: id cua nut nguon
            //Link[index][1]: id cua nut dich
            //Link[index][2]: id cua goi tin
            //Link[index][3] = -1;/*id cua host nguon*/
            //Link[index][4] = -1;/*id cua host dich*/
            //Link[index][5] = 0;/*nix-vector*/
            //Link[index][6] = 0;/*kich thuoc cua goi tin*/
            //Link[index][7] = 0;/*thoi diem bit dau den*/
            //Link[index][8] = 0;/*thoi diem bit cuoi den*/
            //Link[index][9] = 0;/*bandwidth*/
            //Link[index][10] = 0;/*bang 0 nghia la nut nguon ko phai la Host*/
            //Link[index][11] = 0;/*bang 1 nghia la nut dich la Host*/
            //Link[index][12] = 0;//Hop count

            //int **outport; //an array with size = 2 x BUFFER_SIZE
            //outport[0][i]: all ids of packets in outport
            //outport[1][i]: all destinations of packets in outport

            if(avail > 0//Neu co goi tin o top cua outport
                && timeOfC == curr //va goi tin o vi tri top nay co nhu cau duoc gui di
                //&& Hosts[i][3] != -1
                )
            {
                if(credit > 0)//neu inport cua switch tiep theo con cho trong
                {
                    //tao su kien tren link
                    Links[idOfLink][2] = outport[0];//gan id cua goi tin vao Link
                    Links[idOfLink][3] = hostID;//id cua host nguon
                    Links[idOfLink][4] = dstIDs[0];//id cua host dich
                    Links[idOfLink][5] = path[1];//nix vector
                    Links[idOfLink][6] = (int)1e5;
                    Links[idOfLink][7] = (int)(1e9 * Links[idOfLink][6] / Links[idOfLink][9]);
                    Links[idOfLink][8] = 1001;
                    Links[idOfLink][12] = 0;
                    timeOfC = -1;//xoa di thoi diem thuc thi su kien (C)
                    credit --;//thay doi gia tri credit
                    for(j = 0; j < BUFFER_SIZE - 1; j++)
                    {
                        outport[j] = outport[j+1];
                        dstIDs[j] = outport[j+1];
                    }
                    outport[BUFFER_SIZE - 1] = -1;
                    dstIDs[BUFFER_SIZE - 1] = -1;
                    avail--;
                    timeOfB = curr;
                }
            }
        }

        //execute event B             
        if(timeOfB == curr)
        {
            //Check the empty slot in outport
            int slot = BUFFER_SIZE - avail -1;
            

            if(slot > 0)//if there is a empty slot
            {
                outport[avail-1] = q[0]->id;//push the packet in the top source queue to this slot
                dstIDs[avail-1] = q[0]->dstID;
                Queue next = q[0]->next;
                if(next->id == q[1]->id)//if there is zero to one remaining packet in source queue
                {    
                    q[0] = q[0]->next;
                    Queue t = NULL;
                    t = malloc(sizeof(Queue));
                    t->id = -1;
                    t->dstID = -1;
                    t->hostID = -1;
                    t->next = 0;
                    q[1] = t;
                }
                else//otherwise
                {
                    q[0] = q[0]->next;
                }
                if(slot == 0)//neu toan bo outport cua Host trong
                {
                    timeOfC = curr;
                }
            }
            else
            {
                timeOfB = curr + RETRY_TIME;//Khong co outport nao trong, danh phai doi.
            }
        }


        //execute event C
        //NOTE: THE FOLLOWING CODE IS A CLONE OF lines between 103 and 151
        if( Links[idOfLink][2] == -1)//KHONG co goi tin tren duong truyen
        {
            if(avail > 0//Neu co goi tin o top cua outport
                && timeOfC == curr //va goi tin o vi tri top nay co nhu cau duoc gui di
                //&& Hosts[i][3] != -1
                )
            {
                if(credit > 0)//neu inport cua switch tiep theo con cho trong
                {
                    //tao su kien tren link
                    Links[idOfLink][2] = outport[0];//gan id cua goi tin vao Link
                    Links[idOfLink][3] = hostID;//id cua host nguon
                    Links[idOfLink][4] = dstIDs[0];//id cua host dich
                    Links[idOfLink][5] = path[1];//nix vector
                    Links[idOfLink][6] = (int)1e5;
                    Links[idOfLink][7] = (int)(1e9 * Links[idOfLink][6] / Links[idOfLink][9]);
                    Links[idOfLink][8] = 1001;
                    Links[idOfLink][12] = 0;
                    timeOfC = -1;//xoa di thoi diem thuc thi su kien (C)
                    credit --;//thay doi gia tri credit
                    for(j = 0; j < BUFFER_SIZE - 1; j++)
                    {
                        outport[j] = outport[j+1];
                        dstIDs[j] = outport[j+1];
                    }
                    outport[BUFFER_SIZE - 1] = -1;
                    dstIDs[BUFFER_SIZE - 1] = -1;
                    avail--;
                    timeOfB = curr;
                }
            }
        }
        
        
    }
    curr++;
}