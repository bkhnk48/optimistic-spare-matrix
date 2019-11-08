#include <stdio.h>
#include "Init.c"
#include "Host.h"



int main(int argc, char** argv) 
{
    int numOfPorts = 4;
    int numOfSwitches = 20;
    int numOfHosts = 16;
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
    int *Link = NULL;
    Link = malloc(sizeof * Link * (numOfLinks * countOfInfoInLink));

    assignLink(Link, numOfPorts, numOfSwitches, bandwidth);

    int **Hosts = NULL;
    int countOfInfoInHost = 15;
    Hosts = malloc( sizeof * Hosts * numOfHosts);

    for(i = 0; i < numOfHosts; i++)
    {
      Hosts[i] = malloc( sizeof * Hosts[i] * countOfInfoInHost);
    }

    assignHosts(Hosts, numOfHosts, numOfSwitches);
    //show(Hosts, numOfHosts);
    echo(Link, numOfLinks);
    return 0;
}

