#include <stdio.h>
#include "Switch.c"

void showSwitchIDs(int *SwitchIDs, int numOfSwitches);

void showSwitchPacketsAtPort(int **SwitchInportPID, 
                int **SwitchInportSrcIDs, int **SwitchInportDstIDs,
                int bufferSize, int numOfSwitches);

void assignSwitchIDs(int *SwitchIDs, int numOfSwitches);

void assignSwitchPackets(int **SwitchInportPID, 
                int **SwitchInportSrcIDs,
                int **SwitchInportDstIDs,
                int bufferSize, int numOfSwitches);

//void assignEvent(int **SwitchEvtTypes, int **SwitchEvtTimes, 
//                int numOfPorts, int numOfSwitches);

