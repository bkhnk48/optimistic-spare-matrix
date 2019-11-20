#include <stdio.h>
#include "Switch.c"

void showSwitchIDs(int *SwitchIDs, int numOfSwitches);

void showSwitchPacketsAtPort(int **SwitchInportPID, 
                int **SwitchInportSrcIDs, int **SwitchInportDstIDs,
                int bufferSize, int numOfSwitches);

void showEvents(char **SwitchEvtTypes, int **SwitchEvtTimes, 
                int numOfPorts, int numOfSwitches);

void assignSwitchIndexes(int *SwitchIndexes, int *IsHost, int size);

void assignSwitchPackets(int **SwitchInportPID, 
                int **SwitchInportSrcIDs,
                int **SwitchInportDstIDs,
                int bufferSize, int numOfSwitches);

void assignEvents(char **SwitchEvtTypes, int **SwitchEvtTimes, 
                int numOfPorts, int numOfSwitches);




