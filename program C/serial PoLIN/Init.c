#include <stdio.h>
#include "Switches.c"

void display(Host *hosts, int length);
int assignAdj(int **n, int *linkID, int width, int height);
void addLinks(Switch *switches, Host *hosts, Link *links, int *linkID, int height, int width, int numOfHosts);
void createLinkBetweenSwitchHost(Switch aSwitch, Host host, int i, int j, int n);
void createLinkBetweenSwitches(Switch *switches, int i, int j, int n);

void assignAdjant(Switch *switches, Host *hosts, int **n, /* Link *links,*/ int h, int w);

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

void display(Host *hosts, int length)
{
    int i, j, k;
    for(i=0; i<length; i++) 
    {
        j = 0;
        if(hosts[i] != NULL)
        {

            Packet curr = hosts[i]->queue;
            int count = hosts[i]->bufferSize;
            for(k = 0; k < count; k++)
            {
                j++;
                printf("\tFrom %d to %d id = %d||||", curr->src, curr -> dst, curr->id);
                curr = curr->next;
            }
            if(j > 0)
                printf("\n");
        }
        else
        {
            printf("NULL ");
        }
        
    }
}

void assignAdjant(Switch *switches, Host *hosts, int **n, /* Link *links,*/ int h, int w)
{
    int i,j, temp, idOfNode, size;
    size = sizeof(int);
    size = size * 8 - 1;
    int indexOfLink = 0;
    int t;
    
    for(i = 0; i < h; i++)
    {
        for(j = 0; j < w; j++)
        {
            idOfNode = n[i][j];
            temp = idOfNode - h;
            temp = temp >> size;
            
            (switches[i]-> integratedPorts[j]) = malloc(sizeof(IntegratedPort));
            (switches[i]-> integratedPorts[j])->destID = idOfNode;
            (switches[i]-> integratedPorts[j])->linkID = -1;
            switch(temp)
            {
                case 0: //la HOST. GT: temp = 0 nghia la (idOfHost - h) > 0, ma h = No.Switches, tuc la idOfHost > h
                    switches[i]-> host++;
                    hosts[idOfNode - h]->aSwitch = i;
                    (hosts[idOfNode - h] -> outPort) -> destID = i;
                    //Link li = malloc(sizeof(Link));
                    hosts[idOfNode - h]->linkID = indexOfLink;
                    (hosts[idOfNode - h]->outPort)->linkID = indexOfLink;
                    (switches[i]->integratedPorts[j])->linkID = indexOfLink;
                    indexOfLink++;
                    break;
                default: //la Switch. Can kiem tra xem neu Switch[i] ket noi den Switch[j] voi i < j
                    //thi ta tang indexOfLink them 1. Nguoc lai, ko tang
                    t = i - j;//int x = (21 - 17)>>(sizeof(int)*8 - 1); => x = 0|||  x = (11 - 17)>>(sizeof(int)*8 - 1); => x = -1
                    t = t >> size;
                    t = -t;
                    indexOfLink += t;
                    
                    break;
            }
            (switches[i]-> integratedPorts[j])-> creditCount = 5;
            (switches[i]-> integratedPorts[j])->swFlag = 0;
            (switches[i]-> integratedPorts[j])->stFlag = 0;
            if(i == 5 && j == 1)
                printf("\nRTRTYUTYUTY    %d\n", (switches[i]->integratedPorts[j])->destID);
            
        }
    }
}

void createLinkBetweenSwitches(Switch *switches, int i, int j, int n)
{
    int temp = i - n; //i - n[i][j]
    int size = sizeof(int)*8 - 1;
    temp = temp >> size;//neu temp = 0 tuc la i lon hon n[i][j]
    Link t = NULL;
    t = malloc(sizeof(t));
    int idOfSwitch = (switches[i]-> integratedPorts[j])->destID;
    t->idsOfNodes[0] = idOfSwitch;//id cua switch
    t->idsOfNodes[1] = i;//id cua Switch
    t->isBusy[0] = 0;
    t->isBusy[1] = 0;

    switch (temp)
    {
        case 0://Tuc la da co link ton tai giua hai phan tu nay roi
            free(t);
            t = NULL;

            break;
    
        default://nguoc lai i < n[i][j], luu y dau bang ko xay ra
            t->idsOfIntegratedPorts[0] = -1;
            t->idsOfIntegratedPorts[1] = j;
            //switches[n]->link = t;
            //switches[i]->link[j] = t;
            break;
    }
}

void createLinkBetweenSwitchHost(Switch aSwitch, Host host, int i, int j, int n)
{
    //int temp = i - n; //i - n[i][j]
    //int size = sizeof(int)*8 - 1;
    //temp = temp >> size;//neu temp = 0 tuc la i > n[i][j]
    Link t = NULL;
    t = malloc(sizeof(t));
    //int idOfNode = (switches[i]-> integratedPorts[j])->destID;
    int idOfNode = (aSwitch->integratedPorts[j])->destID;
    t->idsOfNodes[0] = idOfNode;//id cua Node (co the Switch hoac Host)
    t->idsOfNodes[1] = i;//id cua Switch
    t->isBusy[0] = 0;
    t->isBusy[1] = 0;

    //switch (temp)
    //{
    //    case 0://Tuc la da co link ton tai giua hai phan tu nay roi
    //        free(t);
    //        t = NULL;

    //        break;
    
    //    default://nguoc lai i < n[i][j], luu y dau bang ko xay ra
            t->idsOfIntegratedPorts[0] = -1;
            t->idsOfIntegratedPorts[1] = j;
            //host->link = t;
            //aSwitch->link[j] = t;
    //        break;
    //}
}


void addLinks(Switch *switches, Host *hosts, Link *links, int *linkID, int height, int width, int numOfHosts)
{
    int i = 0,j = 0, idOfNode = 0;
    int size = sizeof(int);
    size = size * 8 - 1;
    int temp = 0, t = 0;
    
    for(i = 0; i < numOfHosts; i++)
    {
       Link li = malloc(sizeof * li);//Link li = malloc(sizeof(Link));Neu khong viet sizeof * li 
                                     //ma viet sizeof(Link) thi se bi loi malloc(): corrupted top size
       li->idsOfNodes[0] = i + height; 
       li->isBusy[0] = 0; 
       li->isBusy[1] = 0;
       idOfNode = hosts[i] -> aSwitch;
       li->idsOfNodes[1] = idOfNode; 
       li->idsOfIntegratedPorts[0] = -1; 
       li->idsOfIntegratedPorts[1] = 0;
       links[hosts[i]->linkID] = li;
    }
    
    int segment = 0;

    for(i = 0; i < height; i++)
    {
        segment = 0;
        for(j = 0; j < width; j++)
        {
            printf("\n\t Here: i = %d, j = %d ____", i, j);
            
            idOfNode = (switches[i]->integratedPorts[j])->destID;
            if(i == 5 && j == 1)
                printf("\nRTRTYUTYUTY    %d\n", idOfNode);
            temp = idOfNode - height;
            temp = temp >> size;
            t = idOfNode - i;
            t = t >> size;
            //printf("\n\t Here: i = %d, j = %d and (temp + t) = %d at HOST %d", i, j, (temp + t), idOfNode-height);
            switch (temp + t)
            {
                case 0://idOfNode > height > i
                    t = hosts[idOfNode-height]->linkID;
                    
                    (switches[i]->integratedPorts[j]) ->linkID = t;
                    links[t]->idsOfIntegratedPorts[1] = j;
                    //printf("\n\t\t t = %d", t);
                    break;
                case -1: //height > idOfNode > i
                    t = linkID[i] + segment;
                    Link li = malloc(sizeof * li);

                    li->idsOfNodes[0] = idOfNode; 
                    li->idsOfNodes[1] = i; 
                    li->isBusy[0] = 0; 
                    li->isBusy[1] = 0;
                    li->idsOfIntegratedPorts[0] = -1; 
                    li->idsOfIntegratedPorts[1] = j;
                    links[t] = li;
                    (switches[i]->integratedPorts[j]) ->linkID = t;
                    segment++;
                    break;
                case -2://height > i > idOfNode
                    t = idOfNode;
                    links[t] ->idsOfIntegratedPorts[0] = i;
                    (switches[i]->integratedPorts[j]) ->linkID = t;
                    break;
                default:
                    break;
            }
        }
    }
}



int assignAdj(int **n, int *linkID, int width, int height)
{
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

    int i, j;
    int count = 0;
    
    linkID[0] = 0;
    count = width;
    int deltaIndex = width;

    for(i = 1; i < height; i++)
    {
        linkID[i] = deltaIndex + linkID[i - 1];
        deltaIndex = 0;
        for(j = 0; j < width; j++)
        {
            if(i < n[i][j])
            { 
                count++;
                deltaIndex++;
                //linkID[i] += 1;
            }
        }
    }
    
    
    return count; //Could it return 48?

}