#include <stdio.h>
#include "Init.c"
#include "Host.h"
#include "Switch.h"
#include "Link.h"


int main(int argc, char** argv) 
{
    int numOfPorts = 4;
    int numOfSwitches = numOfPorts * numOfPorts * 5 / 4;
    int numOfHosts = numOfPorts * numOfPorts * numOfPorts / 4;
    int numOfInforAboutSwitch = 12;
    int bandwidth = 1000*1000;

    int i, j;

    //De luu thong tin ve link va cac goi du lieu truyen tren link
    //cung nhu cac su kien xay ra tren link
    //ta dung mang 1 chieu nhu sau:
    //  tren mot chieu se co id cua nut nguon, id cua nut dich, id cua goi tin, id cua Host nguon,
    //  id cua Host dich, nix-vector (?), kich thuoc goi tin, bit dau tien cua goi tin se den vao luc nao,
    //  toan bo goi tin se den nut dich khi nao?, bang thong
    int countOfInfoInLink = 10;
    int numOfLinks = numOfSwitches * numOfPorts + numOfHosts;
    int **Links = NULL;
    Links = malloc(sizeof * Links * numOfLinks);

    for(i = 0; i < numOfLinks; i++)
    {
      Links[i] = malloc(sizeof * Links[i] * countOfInfoInLink);
    }

    
    assignLink(Links, numOfPorts);
    showLink(Links, numOfLinks);

    int *IsHost = NULL;
    IsHost = malloc( sizeof * IsHost * (numOfHosts + numOfSwitches)); 
    for(i = 0; i < numOfHosts + numOfSwitches; i++)
    {
      IsHost[i] = 0;
    }

    int **Hosts = NULL;
    int NUM_OF_FIELD_IN_HOST = 16;
    Hosts = malloc( sizeof * Hosts * numOfHosts);

    for(i = 0; i < numOfHosts; i++)
    {
      Hosts[i] = malloc( sizeof * Hosts[i] * NUM_OF_FIELD_IN_HOST);
    }
    assignHosts(Hosts, IsHost, numOfHosts);

    int *SwitchIndexes = NULL;//This array holds the indexes of switch IDs. For example, we have 4 switches with IDs: 4, 8, 9, 10
                              //their indexes are: 1, 2, 3, 4
    int countOfInfoInSwitch = 15;

    SwitchIndexes = malloc( sizeof * SwitchIndexes * (numOfSwitches + numOfHosts));

    assignSwitchIndexes(SwitchIndexes, IsHost, numOfHosts + numOfSwitches);
    //showSwitchIDs(SwitchIndexes, numOfHosts + numOfSwitches);

    int **SwitchPortPID = NULL; //Array stores IDs of packets in inport
    //int **SwitchOutportPID = NULL;//Array stores IDs of packets in outport

    int **SwitchPortSrcIDs = NULL;
    int **SwitchPortDstIDs = NULL;

    //int **SwitchOutportSrcIDs = NULL;
    //int **SwitchOutportDstIDs = NULL;

    int BUFFER_SIZE = 5;

    SwitchPortPID = malloc( sizeof * SwitchPortPID * (numOfSwitches * 2* numOfPorts));
    SwitchPortSrcIDs = malloc( sizeof * SwitchPortSrcIDs * (numOfSwitches * 2 * numOfPorts));
    SwitchPortDstIDs = malloc( sizeof * SwitchPortDstIDs * (numOfSwitches * 2 * numOfPorts));

    for(i = 0; i < numOfSwitches; i++)
    {
      SwitchPortPID[i] = malloc( sizeof * SwitchPortPID[i] * (2*BUFFER_SIZE + 2));
      SwitchPortSrcIDs[i] = malloc( sizeof * SwitchPortSrcIDs[i] * (2*BUFFER_SIZE));
      SwitchPortDstIDs[i] = malloc( sizeof * SwitchPortDstIDs[i] * (2*BUFFER_SIZE));
    }

    char **SwitchEvtTypes = NULL;
    int **SwitchEvtTimes = NULL;//Co 2*k inport, 2*k outport, 2*k channel tai 1 thoi diem
    //Vay co tong cong toi da 2*k + 2*2*k + 2*k = 8*k events tai 1 switch tai 1 thoi diem

    SwitchEvtTypes = malloc( sizeof * SwitchEvtTypes * (numOfSwitches * numOfPorts));
    SwitchEvtTimes = malloc( sizeof * SwitchEvtTimes * (numOfSwitches * numOfPorts));

    for(i = 0; i < numOfSwitches; i++)
    {
      SwitchEvtTypes[i] = malloc(sizeof * SwitchEvtTypes[i] * (8));
      SwitchEvtTimes[i] = malloc(sizeof * SwitchEvtTimes[i] * (8));
    }

    int **switchRT = NULL;
    

    int **Addresses = NULL;
    Addresses = malloc(sizeof * Addresses * (numOfHosts + numOfSwitches));
    for(i = 0; i < numOfSwitches + numOfHosts; i++)
    {
      Addresses[i] = malloc(sizeof * Addresses[i] * 5);
    }

    setAddresses(Addresses, numOfPorts);

    
    //showSwitchIDs(SwitchIDs, numOfSwitches);

    /* assignSwitchPackets(SwitchInportPID, 
                SwitchInportSrcIDs,
                SwitchInportDstIDs,
                BUFFER_SIZE, numOfSwitches);*/
    //printf("Here\n");

    /*assignSwitchPackets(SwitchOutportPID, 
                SwitchOutportSrcIDs,
                SwitchOutportDstIDs,
                BUFFER_SIZE, numOfSwitches);*/

    assignEvents(SwitchEvtTypes, SwitchEvtTimes, numOfPorts, numOfSwitches);

    //showSwitchPacketsAtPort(SwitchInportPID, SwitchInportSrcIDs, 
    //                SwitchInportDstIDs, BUFFER_SIZE, numOfSwitches);

    //showSwitchPacketsAtPort(SwitchOutportPID, SwitchOutportSrcIDs, 
    //                SwitchOutportDstIDs, BUFFER_SIZE, numOfSwitches);
    
    showEvents(SwitchEvtTypes, SwitchEvtTimes, numOfPorts, numOfSwitches);

    
    //show(Hosts, numOfHosts);
    //echo(Link, numOfLinks);
    return 0;
}

