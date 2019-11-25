#include <stdio.h>
#include "RoutingTable.h"

void getNixVector(int src,int dst, RAlgorithm ra, Graph g
                    //, int* hopCount
                    );

int getNextNode(int src, int curr, int dst, RAlgorithm ra, Graph g//, int* hopCount
                    );
                    
void getNixVector(int src,int dst, RAlgorithm ra, Graph g
                        //, int* hopCount
                        )
{
    Step step = NULL;
    step = malloc(sizeof(Step));
    step->node = -1;
    step->port = -1;
    Step tail = NULL;
    int current = src;
    printf("\n%d",src);
    while (current != dst) {
        if (current != src) {
            if(step->node == -1)
            {
                step->node = current;
                tail = step;
            }
            else{
                Step next = NULL;
                next = malloc(sizeof(Step));
                next->node = current;
                tail->next = next;
                tail = next;
            }
        }
        current = getNextNode(src, current, dst, ra, g
                        //, &hopCount
                        );
        printf("->%d", current);
    }
}

int getNextNode(int src, int curr, int dst, RAlgorithm ra, Graph g//, int* hopCount
            )
{
    int adjant = 0, nextOfCore = 0, nextOfAgg = 0;
    int nextOfEdge = 0; int nextIsDst = 0;
    //hopCount++;
    //if (G.isHostVertex(current)) 
    int host = 0;
    host = curr*(g->IsHost[curr]);
    int linkID = g->Hosts[host][12];
    int nextNode = g->Links[linkID][1];
    adjant = (g->IsHost[curr])*nextNode;//tinh ra duoc adjant neu nut curr la nut nguon
    if(g->IsHost[curr]) return nextNode;
    //else
    //if (G.adj(current).contains(destination))
    //neu dst la mot trong so cac nut lien ke cua curr
    int isSwitch = 1 - g->IsHost[curr];//neu curr la Switch thi gia tri nay = 1
    int switchIndex = g->SwitchIndexes[curr*isSwitch];
    int i = 0; int find = 0;
    for(i = 0; i < g->numOfPorts; i++ )
    {
        int less = 1 + ((g->MapFromNodesToPorts[switchIndex][i] - dst)>>(sizeof(int)*8 - 1));
        //Neu a < b thi (a - b)>>(sizeof(int)*8 - 1) = -1, nguoc lai bieu thuc = 0
        //tuc la less = 0 neu g->MapFromNodesToPorts[switchIndex][i] < dst, nguoc lai 1
        int more = 1 + ((dst - g->MapFromNodesToPorts[switchIndex][i])>>(sizeof(int)*8 - 1));
        //Neu a < b thi (a - b)>>(sizeof(int)*8 - 1) = -1, nguoc lai bieu thuc = 0
        //tuc la more = 0 neu dst < g->MapFromNodesToPorts[switchIndex][i], nguoc lai 1
        int sum = (less + more)/2; //= 1 neu dst = g->MapFromNodesToPorts[switchIndex][i], nguoc lai 0
        find += sum;//neu tim thay thi find = 1, khong thay thi find = 0
    }

    nextIsDst = find * dst;//tra ve dst neu nut lien ke cua curr la dst
    if(find) return nextIsDst;


    int addr[4] = {0, 0, 0, 0};
    addr[0] = g->Addresses[dst][0]; addr[1] = g->Addresses[dst][1];
    addr[2] = g->Addresses[dst][2]; addr[3] = g->Addresses[dst][3];

    //if (type == FatTreeGraph.CORE) {
    int isCore = ra->HavingCorePrefix[curr][0];//0 hoac 1
    int isAgg = ra->HavingPrefix[curr][0] + ra->HavingSuffix[curr][0];
    isAgg *= 2; //Vay bang 4 hoac bang 2 hoac bang 0
    int isEdge = ra->HavingSuffix[curr][0]; 
    // //Vay bang 1 hoac bang 0.
    int total = isCore + isAgg;
    int k = g->numOfPorts; int p = 0;
    int nextIsSwitch = 0;
    int coreIndex;
    int delta;
    int edgeIndex;
    int AggIndex;
    switch(total)
    {
        case 1://la Core
            coreIndex = isCore*ra->HavingCorePrefix[curr][1];

            delta = 0;
            for(p = 0; p < k; p++) {
                if(ra->CorePrefix[coreIndex][delta] == addr[0]
                    && ra->CorePrefix[coreIndex][delta + 1] == addr[1]
                    )
                {
                    nextIsSwitch = ra->CorePrefix[coreIndex][delta + 2];
                    return nextIsSwitch;
                    break;
                }
                delta += 3;
            }
            break;
        case 2://La Edge
            edgeIndex = ra->HavingSuffix[curr][1];
            for(p = 0; p < k/2; p++)
            {
                if(ra->Suffix[edgeIndex][p*2] == addr[3])
                { 
                    nextIsSwitch = ra->Suffix[edgeIndex][p*2 + 1];
                    return nextIsSwitch;
                    break;
                }
            }
            break;
        case 4://La Agg
            AggIndex = ra->HavingPrefix[curr][1];
            for (p = 0; p < k / 2; p++) {
                if(ra->Prefix[AggIndex][p*4] == addr[0] && 
                    ra->Prefix[AggIndex][p*4 + 1] == addr[1] &&
                    ra->Prefix[AggIndex][p*4 + 2] == addr[2])
                    return ra->Prefix[AggIndex][p*4 + 3];
            }

            AggIndex = ra->HavingSuffix[curr][1];
            for(p = 0; p < k/2; p++)
            {
                if(ra->Suffix[AggIndex][p*2] == addr[3])
                { 
                    nextIsSwitch = ra->Suffix[AggIndex][p*2 + 1];
                    return nextIsSwitch;
                    break;
                }
            }
            break;
    }
    return -1;
}