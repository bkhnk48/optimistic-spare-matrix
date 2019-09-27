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
		- Simulation running time: about 10 seconds
		- Packet size: 12500 bytes
		- Data rate for packet sending: 1000 Mbps
		- Data rate for device channel: 1000 Mbps
		- Delay time for device: 25e-6 ms or 5e-7 ms
		- Communication pairs selection: Random Selection with uniform probability
		- Traffic flow pattern: CBS
		- Routing protocol: Nix Vector Routing

*/

using namespace ns3;
using namespace std;
NS_LOG_COMPONENT_DEFINE ("Fat-Tree-Architecture");


// Function to create address string from numbers
//
char * toString(int a,int b, int c, int d){

	int first = a;
	int second = b;
	int third = c;
	int fourth = d;

	char *address =  new char[30];
	char firstOctet[30], secondOctet[30], thirdOctet[30], fourthOctet[30];	//address = firstOctet.secondOctet.thirdOctet.fourthOctet;
	
	bzero(address,30);

	snprintf(firstOctet,10,"%d",first);  	strcat(firstOctet,".");
	snprintf(secondOctet,10,"%d",second);   	strcat(secondOctet,".");
	snprintf(thirdOctet,10,"%d",third);   	strcat(thirdOctet,".");
	snprintf(fourthOctet,10,"%d",fourth);

	strcat(thirdOctet,fourthOctet);   	strcat(secondOctet,thirdOctet);
	strcat(firstOctet,secondOctet);   	strcat(address,firstOctet);

	return address;
}

int randBillGen()
{
	//return rand();
	time_t t = time(NULL);
  	struct tm tm = *localtime(&t);
	int lim = tm.tm_year + 1900 + tm.tm_mon + 1 + tm.tm_mday + tm.tm_hour + tm.tm_min + tm.tm_sec;
    static long a = 3;
    a = (((a * 214013L + 2531011L) >> 16) & 32767);
        
    return ((a % lim) + 1);
}

void printTime()
{
	time_t t;
  	time(&t);
  	std::cout<<"\nCurrent date and time: "<<ctime(&t)<<"\n"<<endl;
}

// Main function
//
int 
	main(int argc, char *argv[])
{
//=========== Define parameters based on value of k ===========//
//
	int k;
	

	CommandLine cmd;
    	cmd.AddValue("k", "Number of ports per switch", k);
	cmd.Parse (argc, argv);   // number of ports per switch
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
	// We needed to generate a random number (rn) to be used to eliminate
    // the artificial congestion caused by sending the packets at the
    // same time. This rn is added to AppStartTime to have the sources
    // start at different time, however they will still send at the same rate.
	//std::string command = "echo ";
	int eliminateArtificialCongestion = 1; 
	std::string packetsInQueue = "125000p";
	Config::SetDefault ("ns3::QueueBase::MaxSize", StringValue (packetsInQueue));
	Config::Set ("/NodeList/*/DeviceList/*/TxQueue/MaxSize", StringValue (packetsInQueue));
	Config::SetDefault("ns3::Ipv4GlobalRouting::RandomEcmpRouting",BooleanValue(true));
	
	

	int port = 9;
	int packetSize = 12500 ;		// 12500 bytes
	char MTU_Size [] = "12500" ;    //12500 bytes
	char dataRate_OnOff [] = "1000Mbps";
	char maxBytes [] = "0";		// unlimited
	
// Initialize parameters for Csma and PointToPoint protocol
//
	char dataRate [] = "1000Mbps";	// 1Gbps
	char dataRateCore [] = "1000Mbps";	// 1Gbps
	double link_capacity = 1000*1000; //Kbps	
	double delayBetweenSwitches = 25e-6;//5m / 0.2 (m/ns) = 25e-9 (ns) = 25e-6 (ms)
	double delayHost2Switch = 5e-7;//0.1m/ 0.2 (m/ns) = 5e-10 (ns) = 5e-7 (ms)

	double timeDuration = 1.0; //duration of simulation: 1.0s
	double simulationTime = timeDuration + 2; //timeDuration + 2 = 3s
	
// Output some useful information
//	
	std::cout << "Value of k =  "<< k<<"\n";
	//std::cout << "Total number of hosts =  "<< total_host<<"\n";
	//std::cout << "Number of hosts under each switch =  "<< num_host<<"\n";
	//std::cout << "Number of edge switch under each pod =  "<< num_edge<<"\n";
	//std::cout << "------------- "<<"\n";

// Initialize Internet Stack and Routing Protocols
//	
	InternetStackHelper internet;
	Ipv4NixVectorHelper nixRouting; 
	//Ipv4StaticRoutingHelper staticRouting;
	//Ipv4GlobalRoutingHelper globalRouting;
	Ipv4ListRoutingHelper list;
	//list.Add (staticRouting, 0);	
	list.Add (nixRouting, 0);	
	//list.Add(globalRouting, 10);
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
	ApplicationContainer app[total_host/2];

	int *sources = new int[(total_host/2)];
	int *destinations = new int[(total_host/2)];
	int *allIndexes = new int[total_host];

	for(i = 0; i < total_host/2; i++)
	{
		allIndexes[i*2] = i*2;
		allIndexes[i*2 + 1] = i*2 + 1;
	}

	for (i = 0; i < total_host; i++) {
    	int r = (randBillGen() % (i + 1)); // choose index uniformly in [0, i]
    	int swap = allIndexes[r];
        allIndexes[r] = allIndexes[i];
        allIndexes[i] = swap;
    }

	for(i = 0; i < total_host/2; i++)
	{
		sources[i] = allIndexes[i];
		destinations[i] = allIndexes[i + (total_host/2)];
		//std::cout<<"\t"<<i<<") From source: "<<sources[i]<<" to dest: "<<destinations[i]<<endl;
	}

	delete [] allIndexes;
	allIndexes = NULL;
	
	int server = 0, client = 0;
	int numHostPerPod = k*k/4;

	for (i=0;i<total_host/2;i+= 1){	
		
		//select a server, //int num_pod = k;		// number of pod
		server = sources[i];  //int num_host = (k/2);		// number of hosts under a switch
			//(randBillGen() % (i*2 + 1));
		podRand = server / numHostPerPod;
		swRand = (server - (podRand*numHostPerPod))/num_host;
		hostRand = (server - podRand*numHostPerPod - swRand*num_host);
		hostRand = hostRand + 2;
		
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

		client = destinations[i];
		rand1 = //(client - rand3 - (rand2*num_edge))/num_pod;
				client / numHostPerPod;
		rand2 = (client - (rand1*numHostPerPod))/num_host;
		rand3 = (client - rand1*numHostPerPod - rand2*num_host);
		
	// Install On/Off Application to the client
		NodeContainer onoff;
		onoff.Add(host[rand1][rand2].Get(rand3));
		app[i] = oo.Install (onoff);
		//std::cout << i <<") Data transfer from host["<< podRand <<"]["<< swRand <<"][" << (hostRand - 2) << "] (" <<add<<") to host["<< rand1 << "][" << rand2 << "][" << rand3 <<"]\n";
	}

	//std::cout << "Finished creating On/Off traffic"<<"\n";

	delete [] sources;
	sources = NULL;
	delete [] destinations;
	destinations = NULL;

// Inintialize Address Helper
//	
  	Ipv4AddressHelper address;

// Initialize PointtoPoint helper
//	
	PointToPointHelper p2p;
  	p2p.SetDeviceAttribute ("DataRate", StringValue (dataRate));
  	p2p.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (delayBetweenSwitches)));
	p2p.SetDeviceAttribute ("Mtu", StringValue (MTU_Size));
	
	PointToPointHelper p2pCore;
  	p2pCore.SetDeviceAttribute ("DataRate", StringValue (dataRateCore));
  	p2pCore.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (delayBetweenSwitches)));
	p2pCore.SetDeviceAttribute ("Mtu", StringValue (MTU_Size));

// Initialize Csma helper
//
  	CsmaHelper csma;
  	csma.SetChannelAttribute ("DataRate", StringValue (dataRate));
  	csma.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (delayHost2Switch)));
	csma.SetDeviceAttribute ("EncapsulationMode", StringValue ("Dix"));
	csma.SetDeviceAttribute ("Mtu", StringValue (MTU_Size));
	
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
	//std::cout << "Finished connecting edge switches and hosts  "<< "\n";

//=========== Connect aggregate switches to edge switches ===========//
//
	NetDeviceContainer ae[num_pod][num_agg][num_edge]; 	
	Ipv4InterfaceContainer ipAeContainer[num_pod][num_agg][num_edge];
	for (i=0;i<num_pod;i++){
		for (j=0;j<num_agg;j++){
			for (h=0;h<num_edge;h++){
				ae[i][j][h] = p2pCore.Install(agg[i].Get(j), edge[i].Get(h));

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
	//std::cout << "Finished connecting aggregation switches and edge switches  "<< "\n";

//=========== Connect core switches to aggregate switches ===========//
//
	NetDeviceContainer ca[num_group][num_core][num_pod]; 		
	Ipv4InterfaceContainer ipCaContainer[num_group][num_core][num_pod];
	int fourth_octet =1;
	
	for (i=0; i<num_group; i++){		
		for (j=0; j < num_core; j++){
			fourth_octet = 1;
			for (h=0; h < num_pod; h++){			
				ca[i][j][h] = p2pCore.Install(core[i].Get(j), agg[h].Get(i)); 	
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
	//std::cout << "Finished connecting core switches and aggregation switches  "<< "\n";
	std::cout << "------------- "<<"\n";

//=========== Start the simulation ===========//
//
	std::cout << "Start Simulation.. "<<"\n";

	printTime();

	double maxStopTime = 0;

	for (i=0;i<total_host/2;i++)
	{
		if(eliminateArtificialCongestion)
		{
			Ptr<UniformRandomVariable> x = CreateObject<UniformRandomVariable> ();
    		x->SetAttribute ("Min", DoubleValue (0));
    		x->SetAttribute ("Max", DoubleValue (1));
    		double rn = x->GetValue ();
			app[i].Start (Seconds (rn));
			app[i].Stop (Seconds (rn + timeDuration));
			if(maxStopTime < rn + timeDuration)
			{
				maxStopTime = rn + timeDuration;
			}
		}
		else
		{
			app[i].Start (Seconds (0));
			app[i].Stop (Seconds (timeDuration));
		}
	}

  	Ipv4GlobalRoutingHelper::PopulateRoutingTables ();
// Calculate Throughput using Flowmonitor
//
  	FlowMonitorHelper flowmon;
	Ptr<FlowMonitor> monitor = flowmon.InstallAll();
// Run simulation.
//
  	NS_LOG_INFO ("Run Simulation.");
	simulationTime = maxStopTime + (k);
	Simulator::Stop (Seconds(simulationTime));

	Simulator::Run ();
  	monitor->CheckForLostPackets ();
	Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier> (flowmon.GetClassifier ());
	std::map<FlowId, FlowMonitor::FlowStats> stats = monitor->GetFlowStats ();

	int txPackets=0,rxPackets=0,lostPackets=0;
        ns3::Time delaySum = NanoSeconds(0.0);
        ns3::Time jitterSum = NanoSeconds(0.0);
        ns3::Time lastDelay = NanoSeconds(0.0);
        double timesForwarded=0.0;
        double averageDelay = 0.0;
        double throughput = 0.0; 
		double privateThroughput = 0.0;
	int nFlows=0;
	for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator iter = stats.begin (); iter != stats.end (); ++iter)
  	{
		nFlows++;
		privateThroughput = 0.0;
		txPackets+=iter->second.txPackets;
		rxPackets+=iter->second.rxPackets;
		lostPackets+=iter->second.lostPackets;
		delaySum+=iter->second.delaySum;
		jitterSum+=iter->second.jitterSum;
		lastDelay+=iter->second.lastDelay;
		timesForwarded+=iter->second.timesForwarded;
		if(iter->second.rxPackets != 0)
		{
			averageDelay+=iter->second.delaySum.GetNanoSeconds()/iter->second.rxPackets;
			privateThroughput =iter->second.rxBytes * 8.0 / 
							(iter->second.timeLastRxPacket.GetSeconds()-iter->second.timeFirstTxPacket.GetSeconds());// / 1024;
			throughput += privateThroughput;
		}
		std::cout<<"========================================"<<endl;
	  	std::cout<<"Flow: "<< nFlows<<") has rxPackets: "<<iter->second.rxPackets<<" throughput: "<<(privateThroughput/1024)<<" and current sum: "<<throughput<<endl;
		Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow (iter->first);
        std::cout << "Flow " << iter->first << " (" << t.sourceAddress << " -> " << t.destinationAddress << "): ";
		std::cout<<"\tTime forwarded: "<< iter->second.timesForwarded<<" ";
		std::cout<<"\tLast Rx = "<<iter->second.timeLastRxPacket.GetSeconds()
						<<" First Rx = "<<iter->second.timeFirstRxPacket.GetSeconds()
						<<" First Tx = "<<iter->second.timeFirstTxPacket.GetSeconds()<<endl;
		
    }

  	std::cout<<"Fat-Tree: Simulation in "<<(simulationTime)<< "s";
	if(eliminateArtificialCongestion)
	{
		std::cout<<" and I eliminated artifical congestion" 
		//(like: https://github.com/microsoft/Tocino/blob/master/examples/matrix-topology/matrix-topology.cc) "
			;
	}
	std::cout<<endl;
	
	std::cout<<"\tk = "<<k<<"\n\ttxPackets = "<<txPackets<<"\n\trxPackets = "<<rxPackets<<"\n\tlostPackets = "<<lostPackets<<"\n\tnFlows = "<<nFlows;
	std::cout<<"\n\tpacket size = "<<packetSize<<" (Bytes)"<<endl;
	std::cout<<"\tTime forward = "<<timesForwarded<<endl;
	std::cout<<"\tAverage delay = "<<averageDelay/nFlows<<endl;
	std::cout<<"\tqueue size = "<<packetsInQueue<<endl;
	std::cout<<"\t MTU size = "<<MTU_Size<<endl;
	std::cout<<"\tJitter = "<<(jitterSum/1e9)<<endl;
	std::cout<<"\tthroughput = "<<throughput/(nFlows*1000)<<" (Kbps)"<<endl;
	std::cout<<"\tlink capacity = "<<link_capacity<<" (Kbps)"<<endl;
	std::cout<<"\t% ideal throughput = "<<throughput*100/(1024 * nFlows*link_capacity)<<" %"<<endl;
	if(eliminateArtificialCongestion)
	{
		std::cout<<"\tEliminated artificial congestion: YES"<<endl;
	}
	else{
		std::cout<<"\tEliminated artificial congestion: NO"<<endl;
	}

	printTime();


	std::cout << "Simulation finished "<<"\n";
  	Simulator::Destroy ();
  	NS_LOG_INFO ("Done.");


	return 0;
}


