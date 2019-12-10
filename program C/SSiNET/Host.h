#include<stdio.h>
#include "Host.c"

int getDestination(int id);

void show(int **Hosts, int numOfHosts);

int getHostID(int index);

//void assignHosts(int **Hosts, int *IsHost, int numOfHosts);
void assignHosts(Host *Hosts, int *IsHost, int numOfHosts, int BUFFER_SIZE);

void assignLinkID(Host *Hosts, int **Links, int *IsHost, int numOfLinks, int Bandwidth);

void showHosts(Graph graph);


void setSourceQueue(Queue **sourceQueue, int numOfHosts);


