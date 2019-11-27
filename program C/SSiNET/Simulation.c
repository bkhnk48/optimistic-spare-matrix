#include<stdio.h>
#include<stdlib.h>
#include "Simulation.h"
#include "RoutingPath.h"


void run(Graph g, RAlgorithm ra, int *path, int stop, int curr)
{
    //int curr = 0;
    int i, j;
    int **Hosts = g->Hosts;
    int **Links = g->Links;
    int **SwitchIndexes = g->SwitchIndexes;
    int numOfHosts = g->numOfHosts;
    int BUFFER_SIZE = g->BUFFER_SIZE;
    int CYCLE = g->Hosts[0][0];
    int RETRY_TIME = 3;
    //while(curr < stop)
    {
        for(i = 0; i < numOfHosts; i++)
        {
            //create event A:
            if(curr % CYCLE == 0)
            {
                if(Hosts[i][1] == -1)//if source queue KHONG con goi tin
                {
                    Hosts[i][1] = curr / CYCLE;//curr/CYCLE la id cua goi tin
                    Hosts[i][11] = curr;//create event B
                }
                else
                {
                    Hosts[i][2] = curr / CYCLE; //execute event A
                }   
            }
            //execute event C
            int idOfLink = Hosts[i][12];
            if( Links[idOfLink][2] != -1)//KHONG co goi tin tren duong truyen
            {
                if(Hosts[i][3] != -1)//Neu co goi tin o top cua outport
                {
                    int idNextNode = Links[idOfLink][1];//Lay ra id cua nut tiep theo
                    int nextNodeIndex = SwitchIndexes[idNextNode];
                    //tiep theo can tim ra port nao cua Switch co lien ket den Host
                }
            }
            //execute event B             
            if(Hosts[i][11] == curr)
            {
                //Check the empty slot in outport
                int slot = -1;
                for(j = BUFFER_SIZE - 1; j >= 0; j--)
                {
                    if(Hosts[i][3 + j] == -1)
                    {
                        slot = j;
                        break;
                    }
                }

                if(slot != -1)//if there is a empty slot
                {
                    Hosts[i][3 + slot] = Hosts[i][1];//push the packet to this slot
                    if(Hosts[i][2] == -1)//if there is no remaining packet in source queue
                       Hosts[i][1] = -1;//then the top of source queue is now empty
                    else//otherwise
                    {
                        if(Hosts[i][2] == Hosts[i][1] + 1)//if only one remaining packet is in source queue
                        {
                            Hosts[i][2] = -1;//empty the source queue
                        }
                        Hosts[i][1]++;//push the following packet on the top
                    }
                    if(slot == 0)//neu toan bo outport cua Host trong
                    {
                        Hosts[i][10] = curr;//thiet lap cho viec gui goi tin
                    }
                }
                else
                {
                    Hosts[i][11] == curr + RETRY_TIME;//Khong co outport nao trong, danh phai doi.
                }
            }
            
            
        }
        curr++;
    }
}