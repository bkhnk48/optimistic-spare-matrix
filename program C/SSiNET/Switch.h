#include <stdio.h>
#include "Switch.c"

void showSwitchIDs(int *SwitchIDs, int numOfSwitches);

void showSwitchPacketsAtPort(int **SwitchInportPID, 
                int **SwitchInportSrcIDs, int **SwitchInportDstIDs,
                int bufferSize, int numOfSwitches);

void showEvents(char **SwitchEvtTypes, int **SwitchEvtTimes, 
                int numOfPorts, int numOfSwitches);

void assignSwitchIndexes(int *SwitchIndexes, int *IsHost, int size);

void assignSwitchPackets(int **SwitchPortPID, 
                int **Links,
                int bufferSize, int numOfSwitches
                , int numOfPorts);

void assignEvents(char **SwitchEvtTypes, int **SwitchEvtTimes, 
                int numOfPorts, int numOfSwitches);

void mappingNodeToPort(int **MapFromNodesToPorts, int **Links, 
            int *SwitchIndexes, 
            int numOfLinks, int numOfPorts);

