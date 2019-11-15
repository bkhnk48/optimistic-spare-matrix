#include<stdio.h>

void assignLink(int **Link, int k)
{
    int p = 0, offset = 0, e, edgeSwitch, server, s, a;
    int aggSwitch, c, coreSwitch;
    int numServers = k * k * k / 4;
    int numPodSwitches = k * k;
    // each pod has k^2/4 servers and k switches
    int numEachPod = k * k / 4 + k;
    for (p = 0; p < k; p++) {
        offset = numEachPod * p;
        for (e = 0; e < k / 2; e++) {
            edgeSwitch = offset + k * k / 4 + e;
            // between server and edge
            for (s = 0; s < k / 2; s++) {
                server = offset + e * k / 2 + s;
                addEdge(Link, edgeSwitch, server);
            }
            // between agg and edge
            for (a = k / 2; a < k; a++) {
                aggSwitch = offset + k * k / 4 + a;
                addEdge(Link, edgeSwitch, aggSwitch);
            }
            aggSwitch = offset + k * k / 4 + k / 2 + e;
            for (c = 0; c < k / 2; c++) {
                coreSwitch = e * k / 2 + c + numPodSwitches + numServers;
                addEdge(Link, aggSwitch, coreSwitch);
            }
        }
    }
}

void addEdge(int **Link, int u, int v)
{
    
}