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

        int **allEvents;
                    //allEvents[0]: credit count
                    //allEvents[1]: id of the sent packet
                    //allEvents[2]: id of received packet in next switch
                    //allEvents[3]: time to execute event (I)
                    //allEvents[4]: time to execute event (B)
                    //allEvents[5]: time to execute event (C)
        int **outport; //an array with size = 2 x BUFFER_SIZE
            //outport[0][i]: all ids of packets in outport
            //outport[1][i]: all destinations of packets in outport
    };
    */

    int i, j;
    //Host Hosts = g->Hosts;
    int **Hosts;
    int **Links = g->Links;
    int *SwitchIndexes = g->SwitchIndexes;
    int numOfHosts = g->numOfHosts;
    int BUFFER_SIZE = g->BUFFER_SIZE;
    int CYCLE_PACKET = g->CYCLE_PACKET;
    int dst = path[path[0] - 1];
    int RETRY_TIME = 3;
    //while(curr < stop)
    {
        for(i = 0; i < numOfHosts; i++)
        {
            //create event A:
            if(curr % CYCLE_PACKET == 0)
            {
                if(Hosts[i][1] == -1)//if source queue KHONG con goi tin
                {
                    Hosts[i][1] = curr / CYCLE_PACKET;//curr/CYCLE la id cua goi tin
                    Hosts[i][11] = curr;//create event B
                }
                else
                {
                    Hosts[i][2] = curr / CYCLE_PACKET; //execute event A
                }   
            }
            int credit = Hosts[i][0];
            //execute event C
            int idOfLink = Hosts[i][12];
            if( Links[idOfLink][2] != -1)//KHONG co goi tin tren duong truyen
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

                if(Hosts[i][3] != -1//Neu co goi tin o top cua outport
                    && Hosts[i][10] == curr //va goi tin o vi tri top nay co nhu cau duoc gui di
                    && Hosts[i][3] != -1
                    )
                {
                    if(credit < BUFFER_SIZE)//neu inport cua switch tiep theo con cho trong
                    {
                        //tao su kien tren link
                        Links[idOfLink][2] = Hosts[i][3];//gan id cua goi tin vao Link
                        Links[idOfLink][3] = Hosts[i][14];//id cua host nguon
                        Links[idOfLink][4] = dst;//id cua host dich
                        Links[idOfLink][5] = path[1];//nix vector
                        Links[idOfLink][6] = (int)1e5;
                        Links[idOfLink][7] = (int)(1e9 * Links[idOfLink][6] / Links[idOfLink][9]);
                        Links[idOfLink][8] = 1001;
                        Links[idOfLink][12] = 0;
                        Hosts[i][10] = -1;//xoa di thoi diem thuc thi su kien (C)
                        Hosts[i][3] = -1;//xoa di goi tin o top cua outport
                    }
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