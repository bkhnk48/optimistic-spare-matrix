// Construction of Fat-tree Architecture
// Original Authors: Linh Vu, Daji Wong.
// Updated: Upsana, Suraj : Modified & Extended for Animation and export of statistics
// Updated: ThanhNT

/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2013 Nanyang Technological University 
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>

#include "ns3/flow-monitor-module.h"
#include "ns3/bridge-helper.h"
#include "ns3/bridge-net-device.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/csma-module.h"
#include "ns3/ipv4-nix-vector-helper.h"
#include "ns3/random-variable-stream.h"
#include "ns3/animation-interface.h"

//#define NEED_ANIMATION
//	#define EXPORT_STATS
//#define NEED_TRACE

/*
	- This work goes along with the paper "Towards Reproducible Performance Studies of Datacenter Network Architectures Using An Open-Source Simulation Approach"

	- The code is constructed in the following order:
		1. Creation of Node Containers 
		2. Initialize settings for On/Off Application
		3. Connect hosts to edge switches
		4. Connect edge switches to aggregate switches
		5. Connect aggregate switches to core switches
		6. Start Simulation

	- Addressing scheme:
		1. Address of host: 10.pod.switch.0 /24
		2. Address of edge and aggregation switch: 10.pod.switch.0 /16
		3. Address of core switch: 10.(group number + k).switch.0 /8
		   (Note: there are k/2 group of core switch)

	- On/Off Traffic of the simulation: addresses of client and server are randomly selected everytime
	
	- Simulation Settings:
                - Number of pods (k): 4-24 (run the simulation with varying values of k)
                - Number of nodes: 16-3456
		- Simulation running time: 100 seconds
		- Packet size: 1024 bytes
		- Data rate for packet sending: 1 Mbps
		- Data rate for device channel: 1000 Mbps
		- Delay time for device: 0.001 ms
		- Communication pairs selection: Random Selection with uniform probability
		- Traffic flow pattern: Exponential random traffic
		- Routing protocol: Nix-Vector

        - Statistics Output:
                - Flowmonitor XML output file: Fat-tree.xml is located in the /statistics folder
            

*/

using namespace ns3;
using namespace std;
NS_LOG_COMPONENT_DEFINE ("Fat-Tree-Architecture");

//Function to measure memory usage
//
void process_mem_usage(double& vm_usage, double& resident_set)
{
    vm_usage     = 0.0;
    resident_set = 0.0;

    // the two fields we want
    unsigned long vsize;
    long rss;
    {
        std::string ignore;
        std::ifstream ifs("/proc/self/stat", std::ios_base::in);
        ifs >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore
                >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore
                >> ignore >> ignore >> vsize >> rss;
    }

    long page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024; // in case x86-64 is configured to use 2MB pages
    vm_usage = vsize / 1024.0;
    resident_set = rss * page_size_kb;
}

void printTime()
{
	time_t t;
  time(&t);
  std::cout<<"\nCurrent date and time: "<<ctime(&t)<<"\n"<<endl;
}

// Function to create address string from numbers
//
char * toString(int a,int b, int c, int d){

	int first = a;
	int second = b;
	int third = c;
	int fourth = d;

	char *address =  new char[30];
	char firstOctet[30], secondOctet[30], thirdOctet[30], fourthOctet[30];	
	//address = firstOctet.secondOctet.thirdOctet.fourthOctet;

	bzero(address,30);

	snprintf(firstOctet,10,"%d",first);
	strcat(firstOctet,".");
	snprintf(secondOctet,10,"%d",second);
	strcat(secondOctet,".");
	snprintf(thirdOctet,10,"%d",third);
	strcat(thirdOctet,".");
	snprintf(fourthOctet,10,"%d",fourth);

	strcat(thirdOctet,fourthOctet);
	strcat(secondOctet,thirdOctet);
	strcat(firstOctet,secondOctet);
	strcat(address,firstOctet);

	return address;
}

// Main function
//
int 
	main(int argc, char *argv[])
{
//=========== Define parameters based on value of k ===========//
//
	int k;
	std::string packetsInQueue = "3000p";

	Config::SetDefault ("ns3::QueueBase::MaxSize", StringValue (packetsInQueue));

#ifdef EXPORT_STATS
        ofstream sfile;
 	sfile.open ("statistics/fat-tree-stats.csv", ios::out | ios::app);
#endif
	CommandLine cmd;
    	cmd.AddValue("k", "Number of ports per switch", k);
	cmd.Parse (argc, argv);
	//int k = atoi(argv[0]);			// number of ports per switch
	int num_pod = k;		// number of pod
	int num_host = (k/2);		// number of hosts under a switch
	int num_edge = (k/2);		// number of edge switch in a pod
	int num_bridge = num_edge;	// number of bridge in a pod
	int num_agg = (k/2);		// number of aggregation switch in a pod
	int num_group = k/2;		// number of group of core switches
        int num_core = (k/2);		// number of core switch in a group
	int total_host = k*k*k/4;	// number of hosts in the entire network	
	char filename [] = "statistics/Fat-tree";
	char traceFile [] = "statistics/Fat-tree";
	char buf[4];
	std::sprintf(buf,"-%d",k);
        strcat(filename,buf);
        strcat(filename,".xml");// filename for Flow Monitor xml output file
        strcat(traceFile,buf);
        strcat(traceFile,".tr");// filename for Flow Monitor xml output file

// Define variables for On/Off Application
// These values will be used to serve the purpose that addresses of server and client are selected randomly
// Note: the format of host's address is 10.pod.switch.(host+2)
//
	int podRand = 0;	//	
	int swRand = 0;		// Random values for servers' address
	int hostRand = 0;	//

	int rand1 =0;		//
	int rand2 =0;		// Random values for clients' address	
	int rand3 =0;		//

// Initialize other variables
//
	int i = 0;	
	int j = 0;	
	int h = 0;

// Initialize parameters for On/Off application
//
	int port = 9;
	//Original code
	//int packetSize = 1024;		//Original value: 1024 bytes
	//Endof Original code
	int packetSize = 12500;		// 12500 bytes
	//Original code
	//char dataRate_OnOff [] = "1Mbps";
	//Endof Original code
	char dataRate_OnOff [] = "1000Mbps";
	//char dataRate_OnOff [] = "4Mbps";
	char maxBytes [] = "0";		// unlimited

// Initialize parameters for Csma and PointToPoint protocol
//
	//Original code
	char dataRate [] = "1000Mbps";	// 1Gbps
	//Endof Original code
	//char dataRate [] = "4Mbps";
	double link_capacity = 1000*1000; //Kbps	
	int delay = 0.0001;		// 0.0001 ms. Original: 0.001 ms

	double timeDuration = 1.0; //duration of simulation: 5s
	
// Output some useful information
//	
	std::cout << "Value of k =  "<< k<<"\n";
	std::cout << "Total number of hosts =  "<< total_host<<"\n";
	std::cout << "Number of hosts under each switch =  "<< num_host<<"\n";
	std::cout << "Number of edge switch under each pod =  "<< num_edge<<"\n";
	std::cout << "------------- "<<"\n";

// Initialize Internet Stack and Routing Protocols
//	
	InternetStackHelper internet;
	Ipv4NixVectorHelper nixRouting; 
	Ipv4StaticRoutingHelper staticRouting;
	Ipv4ListRoutingHelper list;
	list.Add (staticRouting, 0);	
	list.Add (nixRouting, 10);	
	internet.SetRoutingHelper(list);

//=========== Creation of Node Containers ===========//
//
	NodeContainer core[num_group];				// NodeContainer for core switches
	for (i=0; i<num_group;i++){  	
		core[i].Create (num_core);
		internet.Install (core[i]);		
	}
	NodeContainer agg[num_pod];				// NodeContainer for aggregation switches
	for (i=0; i<num_pod;i++){  	
		agg[i].Create (num_agg);
		internet.Install (agg[i]);
	}
	NodeContainer edge[num_pod];				// NodeContainer for edge switches
  	for (i=0; i<num_pod;i++){  	
		edge[i].Create (num_bridge);
		internet.Install (edge[i]);
	}
	NodeContainer bridge[num_pod];				// NodeContainer for edge bridges
  	for (i=0; i<num_pod;i++){  	
		bridge[i].Create (num_bridge);
		internet.Install (bridge[i]);
	}
	NodeContainer host[num_pod][num_bridge];		// NodeContainer for hosts
  	for (i=0; i<k;i++){
		for (j=0;j<num_bridge;j++){  	
			host[i][j].Create (num_host);		
			internet.Install (host[i][j]);
		}
	}

//=========== Initialize settings for On/Off Application ===========//
//

// Generate traffics for the simulation
//	
	ApplicationContainer app[total_host];

	int *sources = new int[(total_host/2)];
	int *destinations = new int[(total_host/2)];
	int *allIndexes = new int[total_host];

	for(i = 0; i < total_host/2; i++)
	{
		sources[i] = i;
		destinations[i] = (total_host /2) + i;
		allIndexes[i*2] = i*2;
		allIndexes[i*2 + 1] = i*2 + 1;
	}

	//int r, swap;
	for (i = 0; i < total_host; i++) {
        // choose index uniformly in [0, i]
    	int r = (rand() % (i + 1));
    	int swap = allIndexes[r];
        allIndexes[r] = allIndexes[i];
        allIndexes[i] = swap;
    }

	//for(i = 0; i < total_host; i++)
	//{
	//	std::cout<<"\tallIndexes["<<i<<"]="<<allIndexes[i]<<endl;
	//}
	
	int hostIndex = 0;
	for (i=0;i<total_host;i+= 2){	
		//int num_pod = k;		// number of pod
		//int num_host = (k/2);		// number of hosts under a switch
		//int num_edge = (k/2);		// number of edge switch in a pod

		//select a server
		hostRand = i % num_host;
		hostIndex = i - hostRand;
		hostRand = hostRand + 2;

		hostIndex = (hostIndex*num_edge)/k;
		swRand = hostIndex % num_edge;
		
		hostIndex = hostIndex - swRand;
		podRand = hostIndex / num_edge;
		
		char *add;
		add = toString(10, podRand, swRand, hostRand);

	// Initialize On/Off Application with addresss of server
		OnOffHelper oo = OnOffHelper("ns3::UdpSocketFactory",Address(InetSocketAddress(Ipv4Address(add), port))); // ip address of server
	        
			oo.SetAttribute("OnTime",StringValue ("ns3::ConstantRandomVariable[Constant=1]"));  
	        oo.SetAttribute("OffTime",StringValue ("ns3::ConstantRandomVariable[Constant=0]"));    
			 
 	        oo.SetAttribute("PacketSize",UintegerValue (packetSize));
 	       	oo.SetAttribute("DataRate",StringValue (dataRate_OnOff));      
	        oo.SetAttribute("MaxBytes",StringValue (maxBytes));

	// Randomly select a client
		//rand1 = podRand + (num_pod/2);
		//rand2 = swRand;
		//rand3 = hostRand - 2;
		rand1 = podRand;
		rand2 = swRand;
		rand3 = hostRand - 1;
		

	// Install On/Off Application to the client
		NodeContainer onoff;
		onoff.Add(host[rand1][rand2].Get(rand3));
	    app[i] = oo.Install (onoff);
		//ThanhNT: 17-09-19: 16:28
		std::cout << (i/2+ 1) <<") Data transfer from host["<< podRand <<"]["<< swRand <<"][" << (hostRand - 2) << "] to host["<< rand1 << "][" << rand2 << "][" << rand3 <<"]\n";
		//endof ThanhNT
	}
	std::cout << "Finished creating On/Off traffic"<<"\n";

// Inintialize Address Helper
//	
  	Ipv4AddressHelper address;

// Initialize PointtoPoint helper
//	
	PointToPointHelper p2p;
  	p2p.SetDeviceAttribute ("DataRate", StringValue (dataRate));
  	p2p.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (delay)));

// Initialize Csma helper
//
  	CsmaHelper csma;
  	csma.SetChannelAttribute ("DataRate", StringValue (dataRate));
  	csma.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (delay)));

//=========== Connect edge switches to hosts ===========//
//	
	NetDeviceContainer hostSw[num_pod][num_bridge];		
	NetDeviceContainer bridgeDevices[num_pod][num_bridge];	
	Ipv4InterfaceContainer ipContainer[num_pod][num_bridge];

	for (i=0;i<num_pod;i++){
		for (j=0;j<num_bridge; j++){
			NetDeviceContainer link1 = csma.Install(NodeContainer (edge[i].Get(j), bridge[i].Get(j)));
			hostSw[i][j].Add(link1.Get(0));				
			bridgeDevices[i][j].Add(link1.Get(1));			

			for (h=0; h< num_host;h++){			
				NetDeviceContainer link2 = csma.Install (NodeContainer (host[i][j].Get(h), bridge[i].Get(j)));
				hostSw[i][j].Add(link2.Get(0));			
				bridgeDevices[i][j].Add(link2.Get(1));						
			}

			BridgeHelper bHelper;
			bHelper.Install (bridge[i].Get(j), bridgeDevices[i][j]);
			//Assign address
			char *subnet;
			subnet = toString(10, i, j, 0);
			address.SetBase (subnet, "255.255.255.0");
			ipContainer[i][j] = address.Assign(hostSw[i][j]);			
		}
	}
	std::cout << "Finished connecting edge switches and hosts  "<< "\n";

//=========== Connect aggregate switches to edge switches ===========//
//
	NetDeviceContainer ae[num_pod][num_agg][num_edge]; 	
	Ipv4InterfaceContainer ipAeContainer[num_pod][num_agg][num_edge];
	for (i=0;i<num_pod;i++){
		for (j=0;j<num_agg;j++){
			for (h=0;h<num_edge;h++){
				ae[i][j][h] = p2p.Install(agg[i].Get(j), edge[i].Get(h));

				int second_octet = i;		
				int third_octet = j+(k/2);	
				int fourth_octet;
				if (h==0) fourth_octet = 1;
				else fourth_octet = h*2+1;
				//Assign subnet
				char *subnet;
				subnet = toString(10, second_octet, third_octet, 0);
				//Assign base
				char *base;
				base = toString(0, 0, 0, fourth_octet);
				address.SetBase (subnet, "255.255.255.0",base);
				ipAeContainer[i][j][h] = address.Assign(ae[i][j][h]);
			}			
		}		
	}
	std::cout << "Finished connecting aggregation switches and edge switches  "<< "\n";

//=========== Connect core switches to aggregate switches ===========//
//
	NetDeviceContainer ca[num_group][num_core][num_pod]; 		
	Ipv4InterfaceContainer ipCaContainer[num_group][num_core][num_pod];
	int fourth_octet =1;
	
	for (i=0; i<num_group; i++){		
		for (j=0; j < num_core; j++){
			fourth_octet = 1;
			for (h=0; h < num_pod; h++){			
				ca[i][j][h] = p2p.Install(core[i].Get(j), agg[h].Get(i)); 	

				int second_octet = k+i;		
				int third_octet = j;
				//Assign subnetr
				char *subnet;
				subnet = toString(10, second_octet, third_octet, 0);
				//Assign base
				char *base;
				base = toString(0, 0, 0, fourth_octet);
				address.SetBase (subnet, "255.255.255.0",base);
				ipCaContainer[i][j][h] = address.Assign(ca[i][j][h]);
				fourth_octet +=2;
			}
		}
	}
	std::cout << "Finished connecting core switches and aggregation switches  "<< "\n";
	std::cout << "------------- "<<"\n";

//=========== Start the simulation ===========//
//
	std::cout << "Start Simulation.. "<<"\n";

	printTime();

	for (i=0;i<total_host;i++){
		app[i].Start (Seconds (0.0));
		//Code cu chuan
  		//app[i].Stop (Seconds (101.0));
		//Endof Code cu chuan
		//ThanhNT 13-09-12:10
		//app[i].Stop (Seconds (11.0));
		app[i].Stop (Seconds (timeDuration + 1));
		//Endof ThanhNT 13-09-12:10
	}
  	Ipv4GlobalRoutingHelper::PopulateRoutingTables ();
// Calculate Throughput using Flowmonitor
//
  	FlowMonitorHelper flowmon;
	Ptr<FlowMonitor> monitor = flowmon.InstallAll();
// Run simulation.
//
  	NS_LOG_INFO ("Run Simulation.");
	//Code cu chuan
  	//Simulator::Stop (Seconds(100.0));
	//Endof Code cu chuan
		//Simulator::Stop (Seconds(1.0));
		//Simulator::Stop (Seconds(10.0));
		Simulator::Stop (Seconds(timeDuration));
#ifdef NEED_TRACE
  	AsciiTraceHelper ascii;
  	csma.EnableAsciiAll (ascii.CreateFileStream (traceFile));
#endif


	Simulator::Run ();
  	monitor->CheckForLostPackets ();
	Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier> (flowmon.GetClassifier ());
	std::map<FlowId, FlowMonitor::FlowStats> stats = monitor->GetFlowStats ();

	int txPackets=0,rxPackets=0,lostPackets=0;
        ns3::Time delaySum = NanoSeconds(0.0);
        ns3::Time jitterSum = NanoSeconds(0.0);
        ns3::Time lastDelay = NanoSeconds(0.0);
        int timesForwarded=0;
        double averageDelay;
        double throughput; 
	int nFlows=0;
	for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator iter = stats.begin (); iter != stats.end (); ++iter)
  	{
	  nFlows++;
	  txPackets+=iter->second.txPackets;
	  rxPackets+=iter->second.rxPackets;
	  lostPackets+=iter->second.lostPackets;
	  delaySum+=iter->second.delaySum;
	  jitterSum+=iter->second.jitterSum;
	  lastDelay+=iter->second.lastDelay;
	  timesForwarded+=iter->second.timesForwarded;
	  averageDelay+=iter->second.delaySum.GetNanoSeconds()/iter->second.rxPackets;
	  throughput+=iter->second.rxBytes * 8.0 / (iter->second.timeLastRxPacket.GetSeconds()-iter->second.timeFirstTxPacket.GetSeconds());// / 1024;
    }

  	std::cout<<"Fat-Tree: Simulation in "<<timeDuration<< "s"<<endl;
	std::cout<<"\tk = "<<k<<"\n\ttxPackets = "<<txPackets<<"\n\trxPackets = "<<rxPackets<<"\n\tlostPackets = "<<lostPackets<<"\n\tnFlows = "<<nFlows;
	std::cout<<"\n\tpacket size = "<<packetSize<<" (Bytes)"<<endl;
	std::cout<<"\tdelay = "<<(delay*1000 * 1000)<<" (nano-sec)"<<endl;
	std::cout<<"\tqueue size = "<<packetsInQueue<<endl;
	std::cout<<"\tthroughput = "<<throughput/nFlows/1024<<" (Kbps)"<<endl;
	std::cout<<"\tlink capacity = "<<link_capacity<<" (Kbps)"<<endl;
	std::cout<<"\talternative throughput = "<<throughput*100/(1024 * nFlows*link_capacity)<<" %"<<endl;
	std::cout<<"\t 3.000 packets in queue size results in 93% of throughput"<<endl;
	

	printTime();

  //double vm, rss;
  //process_mem_usage(vm, rss);
  //std::cout << "VM: " << (vm/1024) << "; RSS: " << rss << std::endl;

	std::cout << "Simulation finished "<<"\n";
  	Simulator::Destroy ();
  	NS_LOG_INFO ("Done.");
	return 0;
}

