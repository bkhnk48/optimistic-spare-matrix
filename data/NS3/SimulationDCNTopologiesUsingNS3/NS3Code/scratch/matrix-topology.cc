/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 Egemen K. Cetinkaya, Justin P. Rohrer, and Amit Dandekar
 * Modification: NGUYEN TIEN THANH
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
 * Author: Egemen K. Cetinkaya <ekc@ittc.ku.edu>
 * Author: Justin P. Rohrer    <rohrej@ittc.ku.edu>
 * Author: Amit Dandekar       <dandekar@ittc.ku.edu>
 *
 * James P.G. Sterbenz <jpgs@ittc.ku.edu>, director
 * ResiliNets Research Group  http://wiki.ittc.ku.edu/resilinets
 * Information and Telecommunication Technology Center 
 * and
 * Department of Electrical Engineering and Computer Science
 * The University of Kansas
 * Lawrence, KS  USA
 *
 * Work supported in part by NSF FIND (Future Internet Design) Program
 * under grant CNS-0626918 (Postmodern Internet Architecture) and 
 * by NSF grant CNS-1050226 (Multilayer Network Resilience Analysis and Experimentation on GENI)
 *
 * This program reads an upper triangular adjacency matrix (e.g. RSN_adjacency_matrix.txt) and
 * node coordinates file (e.g. RSN_node_coordinates.txt). The program also set-ups a
 * wired network topology with P2P links according to the adjacency matrix with
 * k*k*k/8 CBR traffic flows, in which k is the number of ports inside a node.
 * Keep in mind that number of hosts and switches are k*k*k/4 and k*k*k/5 respectively
 */

// ---------- Header Includes -------------------------------------------------
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/global-route-manager.h"
#include "ns3/mobility-module.h"
#include "ns3/netanim-module.h"
#include "ns3/assert.h"
#include "ns3/ipv4-global-routing-helper.h"

#include "ns3/flow-monitor-module.h"

using namespace std;
using namespace ns3;

// ---------- Prototypes ------------------------------------------------------

vector<vector<bool> > readNxNMatrix (std::string adj_mat_file_name);
vector<vector<double> > readCordinatesFile (std::string node_coordinates_file_name);
void printCoordinateArray (const char* description, vector<vector<double> > coord_array);
void printMatrix (const char* description, vector<vector<bool> > array);
int randBillGen();
void printTime();
void SinkRxTrace(Ptr<const Packet> pkt, const Address &addr);

double interval = 0.01; //0.01s
double *bytesPeriod ; //= new double[600];
std::string path = "/home/tienthanh/Public/NS3repo/ns-3-allinone/ns-3-dev/scratch/subdir/";
double throughput = 0.0; 



NS_LOG_COMPONENT_DEFINE ("GenericTopologyCreation");

int main (int argc, char *argv[])
{

  // ---------- Simulation Variables ------------------------------------------

  // Change the variables and file names only in this block!

  double SimTime        = 4.00; //the original value is 13
  double SinkStartTime  = 1.0001;
  double SinkStopTime   = 2.90001;
  double AppStartTime   = 2.0001;
  double AppStopTime    = 2.80001;

  std::string AppPacketRate ("1000Mbps");

  //ThanhNT 15-10-19 some configurations
  int k;
	//int eliminateArtificialCongestion = 0; 	
	//int timeOfRun = 0;

  int i = 0;

	CommandLine cmd;
    cmd.AddValue("k", "Number of ports per switch", k);
		//cmd.AddValue("EAC", "Eliminate Artificial Congestion or NOT", eliminateArtificialCongestion);
		//cmd.AddValue("i", "Time of Running", timeOfRun);
	cmd.Parse (argc, argv);   // number of ports per switch

  std::string packetsInQueue = "125000p";
	//std::string packetsInQueue = "5p";
	Config::SetDefault ("ns3::QueueBase::MaxSize", StringValue (packetsInQueue));
	Config::Set ("/NodeList/*/DeviceList/*/TxQueue/MaxSize", StringValue (packetsInQueue));
	Config::Set ("/NodeList/*/DeviceList/*/RxQueue/MaxSize", StringValue (packetsInQueue));
	Config::SetDefault("ns3::Ipv4GlobalRouting::RandomEcmpRouting",BooleanValue(true));
  char maxBytes [] = "0";		// unlimited
  int num_switches = k*k*5/4;
  //ThanhNT 15-10-19 some configurations


  Config::SetDefault  ("ns3::OnOffApplication::PacketSize",StringValue ("12500"));
  Config::SetDefault ("ns3::OnOffApplication::DataRate",  StringValue (AppPacketRate));
  std::string LinkRate ("1000Mbps");
  //std::string LinkDelay ("2ms");
  char MTU_Size [] = "12500" ;    //12500 bytes
  double delayBetweenSwitches = 5e-6;//1m / 0.2 (m/ns) = 5e-9 (ns) = 5e-6 (ms)

  //  DropTailQueue::MaxPackets affects the # of dropped packets, default value:100
  //  Config::SetDefault ("ns3::DropTailQueue::MaxPackets", UintegerValue (1000));

  srand ( (unsigned)time ( NULL ) );   // generate different seed each time

  //std::string tr_name ("n-node-ppp.tr");
  //std::string pcap_name ("n-node-ppp");
  //std::string flow_name ("n-node-ppp.xml");
  //std::string anim_name ("n-node-ppp.anim.xml");

  double link_capacity = 1000*1000;

  //std::string adj_mat_file_name ("/home/tienthanh/Public/NS3repo/ns-3-allinone/ns-3-dev/scratch/subdir/matrixTopo/adjacency_matrix.txt");
  //std::string node_coordinates_file_name ("/home/tienthanh/Public/NS3repo/ns-3-allinone/ns-3-dev/scratch/subdir/matrixTopo/node_coordinates.txt");
  std::string adj_mat_file_name ("/home/tienthanh/Public/NS3repo/ns-3-allinone/ns-3-dev/scratch/subdir/matrixTopo/RSN_adjacency_matrix.txt");
  std::string node_coordinates_file_name ("/home/tienthanh/Public/NS3repo/ns-3-allinone/ns-3-dev/scratch/subdir/matrixTopo/RSN_node_coordinates.txt");


  // ---------- End of Simulation Variables ----------------------------------

  // ---------- Read Adjacency Matrix ----------------------------------------

  vector<vector<bool> > Adj_Matrix;
  Adj_Matrix = readNxNMatrix (adj_mat_file_name);

  // Optionally display 2-dimensional adjacency matrix (Adj_Matrix) array
  // printMatrix (adj_mat_file_name.c_str (),Adj_Matrix);

  // ---------- End of Read Adjacency Matrix ---------------------------------

  // ---------- Read Node Coordinates File -----------------------------------

  vector<vector<double> > coord_array;
  coord_array = readCordinatesFile (node_coordinates_file_name);

  // Optionally display node co-ordinates file
  // printCoordinateArray (node_coordinates_file_name.c_str (),coord_array);

  int n_nodes = coord_array.size ();
  int matrixDimension = Adj_Matrix.size ();

  if (matrixDimension != n_nodes)
    {
      NS_FATAL_ERROR ("The number of lines in coordinate file is: " << n_nodes << " not equal to the number of nodes in adjacency matrix size " << matrixDimension);
    }

  // ---------- End of Read Node Coordinates File ----------------------------

  // ---------- Network Setup ------------------------------------------------

  NS_LOG_INFO ("Create Nodes.");

  NodeContainer nodes;   // Declare nodes objects
  nodes.Create (n_nodes);

  NS_LOG_INFO ("Create P2P Link Attributes.");

  PointToPointHelper p2p;
  p2p.SetDeviceAttribute ("DataRate", StringValue (LinkRate));
  //ThanhNT adjust properties of link connection:
  //(i) add MTU size = 12500
  //(ii) fix the LinkDelay = 5e-6
  //To do that, first set comment the following statement and then add two new statements
  //p2p.SetChannelAttribute ("Delay", StringValue (LinkDelay));
  p2p.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (delayBetweenSwitches)));
  p2p.SetDeviceAttribute ("Mtu", StringValue (MTU_Size));
  //Endof ThanhNT adjust properties of link connection:

  NS_LOG_INFO ("Install Internet Stack to Nodes.");

  InternetStackHelper internet;
  internet.Install (NodeContainer::GetGlobal ());

  NS_LOG_INFO ("Assign Addresses to Nodes.");

  Ipv4AddressHelper ipv4_n;
  ipv4_n.SetBase ("10.0.0.0", "255.255.255.252");

  NS_LOG_INFO ("Create Links Between Nodes.");

  uint32_t linkCount = 0;

  for (size_t i = 0; i < Adj_Matrix.size (); i++)
    {
      for (size_t j = 0; j < Adj_Matrix[i].size (); j++)
        {

          if (Adj_Matrix[i][j] == 1)
            {
              NodeContainer n_links = NodeContainer (nodes.Get (i), nodes.Get (j));
              NetDeviceContainer n_devs = p2p.Install (n_links);
              ipv4_n.Assign (n_devs);
              ipv4_n.NewNetwork ();
              linkCount++;
              NS_LOG_INFO ("matrix element [" << i << "][" << j << "] is 1");
            }
          else
            {
              NS_LOG_INFO ("matrix element [" << i << "][" << j << "] is 0");
            }
        }
    }
  NS_LOG_INFO ("Number of links in the adjacency matrix is: " << linkCount);
  NS_LOG_INFO ("Number of all nodes is: " << nodes.GetN ());

  NS_LOG_INFO ("Initialize Global Routing.");
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  // ---------- End of Network Set-up ----------------------------------------

  // ---------- Allocate Node Positions --------------------------------------

  NS_LOG_INFO ("Allocate Positions to Nodes.");

  MobilityHelper mobility_n;
  Ptr<ListPositionAllocator> positionAlloc_n = CreateObject<ListPositionAllocator> ();

  for (size_t m = 0; m < coord_array.size (); m++)
    {
      positionAlloc_n->Add (Vector (coord_array[m][0], coord_array[m][1], 0));
      Ptr<Node> n0 = nodes.Get (m);
      Ptr<ConstantPositionMobilityModel> nLoc =  n0->GetObject<ConstantPositionMobilityModel> ();
      if (nLoc == 0)
        {
          nLoc = CreateObject<ConstantPositionMobilityModel> ();
          n0->AggregateObject (nLoc);
        }
      // y-coordinates are negated for correct display in NetAnim
      // NetAnim's (0,0) reference coordinates are located on upper left corner
      // by negating the y coordinates, we declare the reference (0,0) coordinate
      // to the bottom left corner
      Vector nVec (coord_array[m][0], -coord_array[m][1], 0);
      nLoc->SetPosition (nVec);

    }
  mobility_n.SetPositionAllocator (positionAlloc_n);
  mobility_n.Install (nodes);

  // ---------- End of Allocate Node Positions -------------------------------

  // ---------- Create k*k*k/8 CBR Flows -------------------------------------

  NS_LOG_INFO ("Setup Packet Sinks.");

  uint16_t port = 9;

  for (int i = num_switches; i < n_nodes; i++)
    {
      PacketSinkHelper sink ("ns3::UdpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), port));
      ApplicationContainer apps_sink = sink.Install (nodes.Get (i));   // sink is installed on all nodes
      apps_sink.Start (Seconds (SinkStartTime));
      apps_sink.Stop (Seconds (SinkStopTime));
    }

  NS_LOG_INFO ("Setup CBR Traffic Sources.");

  int total_host = n_nodes - num_switches;
  //std::cout<<"total_host: "<<total_host<<endl;
  int *sources = new int[(total_host/2)];
	int *destinations = new int[(total_host/2)];
	int *allIndexes = new int[total_host];

	for(i = 0; i < total_host/2; i++)
	{
		allIndexes[i*2] = i*2 + num_switches;
		allIndexes[i*2 + 1] = i*2 + 1 + num_switches;
	}

	for (i = 0; i < total_host; i++) {
    	int r = (randBillGen() % (i + 1)); // choose index uniformly in [0, i]
      //std::cout<<"\tr = "<<r<<endl;
    	int swap = allIndexes[r];
        allIndexes[r] = allIndexes[i];
        allIndexes[i] = swap;
    }

	for(i = 0; i < total_host/2; i++)
	{
		sources[i] = allIndexes[i];
		destinations[i] = allIndexes[i + (total_host/2)];
	}

	delete [] allIndexes;
	allIndexes = NULL;

  //for (int i = 0; i < n_nodes; i++)
  for (int i = 0; i < total_host/2; i++)
    {
      //for (int j = 0; j < n_nodes; j++)
        {
          //if (i != j)
            {

              // We needed to generate a random number (rn) to be used to eliminate
              // the artificial congestion caused by sending the packets at the
              // same time. This rn is added to AppStartTime to have the sources
              // start at different time, however they will still send at the same rate.

              Ptr<UniformRandomVariable> x = CreateObject<UniformRandomVariable> ();
              x->SetAttribute ("Min", DoubleValue (0));
              x->SetAttribute ("Max", DoubleValue (1));
              double rn = x->GetValue ();
              //Ptr<Node> n = nodes.Get (j);
              Ptr<Node> n = nodes.Get(destinations[i]);
              Ptr<Ipv4> ipv4 = n->GetObject<Ipv4> ();
              Ipv4InterfaceAddress ipv4_int_addr = ipv4->GetAddress (1, 0);
              Ipv4Address ip_addr = ipv4_int_addr.GetLocal ();
              OnOffHelper onoff ("ns3::UdpSocketFactory", InetSocketAddress (ip_addr, port)); // traffic flows from node[i] to node[j]
              //ThanhNT add new settings for OnOfHelper
              onoff.SetAttribute("OnTime",StringValue ("ns3::ConstantRandomVariable[Constant=1]")); 
	            onoff.SetAttribute("OffTime",StringValue ("ns3::ConstantRandomVariable[Constant=0]"));    
	            onoff.SetAttribute("MaxBytes",StringValue (maxBytes));
              //Endof ThanhNT add new settings for OnOfHelper

              onoff.SetConstantRate (DataRate (AppPacketRate));
              //ApplicationContainer apps = onoff.Install (nodes.Get (i));  // traffic sources are installed on all nodes
              ApplicationContainer apps = onoff.Install (nodes.Get (sources[i]));
              apps.Start (Seconds (AppStartTime + rn));
              apps.Stop (Seconds (AppStopTime));

              
            }
        }
    }

  // ---------- End of Create n*(n-1) CBR Flows ------------------------------

  // ---------- Simulation Monitoring ----------------------------------------

  NS_LOG_INFO ("Configure Tracing.");

  //ThanhNT reduce writing efforts
  //by setting TWO following statements to comments
  //AsciiTraceHelper ascii;
  //p2p.EnableAsciiAll (ascii.CreateFileStream (tr_name.c_str ()));
  //Endof ThanhNT reduce writing efforts

  // p2p.EnablePcapAll (pcap_name.c_str());

  Ptr<FlowMonitor> monitor;
  FlowMonitorHelper flowmonHelper;
  monitor = flowmonHelper.InstallAll ();

  // Configure animator with default settings

  //ThanhNT reduce writing efforts
  //by setting THE following statement to comment
  //AnimationInterface anim (anim_name.c_str ());
  //Endof ThanhNT reduce writing efforts
  NS_LOG_INFO ("Run Simulation.");
  
  Simulator::Stop (Seconds (SimTime));

  std::cout << "Start Simulation.. will be ended after "<<SimTime<<" (s)\n";

	printTime();

  Simulator::Run ();

  monitor->CheckForLostPackets ();

	Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier> (flowmonHelper.GetClassifier ());
	std::map<FlowId, FlowMonitor::FlowStats> stats = monitor->GetFlowStats ();

	int txPackets=0,rxPackets=0,lostPackets=0;
        ns3::Time delaySum = NanoSeconds(0.0);
        ns3::Time jitterSum = NanoSeconds(0.0);
        ns3::Time lastDelay = NanoSeconds(0.0);
        double timesForwarded=0.0;
        double averageDelay = 0.0;
        
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
		//else
    {
			std::cout<<"========================================"<<endl;
	  		std::cout<<"Flow: "<< nFlows<<") has rxPackets: "<<iter->second.rxPackets<<" throughput: "<<(privateThroughput/1024)<<" and current sum: "<<throughput<<endl;
			Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow (iter->first);
        	std::cout << "Flow " << iter->first << " (" << t.sourceAddress << " -> " << t.destinationAddress << "): ";
			std::cout<<"\tTime forwarded: "<< iter->second.timesForwarded<<" ";
			std::cout<<"\tLast Rx = "<<iter->second.timeLastRxPacket.GetSeconds()
						<<" First Rx = "<<iter->second.timeFirstRxPacket.GetSeconds()
						<<" First Tx = "<<iter->second.timeFirstTxPacket.GetSeconds()<<endl;
		}
		
  }

  
	
	std::cout<<"\n\ttxPackets = "<<txPackets<<"\n\trxPackets = "<<rxPackets<<"\n\tlostPackets = "<<lostPackets<<"\n\tnFlows = "<<nFlows;
	std::cout<<"\n\tTime forward = "<<timesForwarded<<" or"<<endl;
	std::cout<<"\tAverage delay = "<<averageDelay/nFlows<<endl;
	//std::cout<<"\tJitter = "<<(jitterSum/1e9)<<endl;
	std::cout<<"\tthroughput = "<<throughput/(nFlows*1000)<<" (Kbps)"<<endl;
	std::cout<<"\tlink capacity = "<<link_capacity<<" (Kbps)"<<endl;
	std::cout<<"\t% throughput = "<<throughput*100/(1024 * nFlows*link_capacity)<<" %"<<endl;
	
  // flowmon->SerializeToXmlFile (flow_name.c_str(), true, true);
  printTime();
	std::cout << "Simulation finished "<<"\n";
  Simulator::Destroy ();

  // ---------- End of Simulation Monitoring ---------------------------------

  return 0;

}

// ---------- Function Definitions -------------------------------------------

vector<vector<bool> > readNxNMatrix (std::string adj_mat_file_name)
{
  ifstream adj_mat_file;
  adj_mat_file.open (adj_mat_file_name.c_str (), ios::in);
  if (adj_mat_file.fail ())
    {
      NS_FATAL_ERROR ("File " << adj_mat_file_name.c_str () << " not found");
    }
  vector<vector<bool> > array;
  int i = 0;
  int n_nodes = 0;

  while (!adj_mat_file.eof ())
    {
      string line;
      getline (adj_mat_file, line);
      if (line == "")
        {
          NS_LOG_WARN ("WARNING: Ignoring blank row in the array: " << i);
          break;
        }

      istringstream iss (line);
      bool element;
      vector<bool> row;
      int j = 0;

      while (iss >> element)
        {
          row.push_back (element);
          j++;
        }

      if (i == 0)
        {
          n_nodes = j;
        }

      if (j != n_nodes )
        {
          NS_LOG_ERROR ("ERROR: Number of elements in line " << i << ": " << j << " not equal to number of elements in line 0: " << n_nodes);
          NS_FATAL_ERROR ("ERROR: The number of rows is not equal to the number of columns! in the adjacency matrix");
        }
      else
        {
          array.push_back (row);
        }
      i++;
    }

  if (i != n_nodes)
    {
      NS_LOG_ERROR ("There are " << i << " rows and " << n_nodes << " columns.");
      NS_FATAL_ERROR ("ERROR: The number of rows is not equal to the number of columns! in the adjacency matrix");
    }

  adj_mat_file.close ();
  return array;

}

vector<vector<double> > readCordinatesFile (std::string node_coordinates_file_name)
{
  ifstream node_coordinates_file;
  node_coordinates_file.open (node_coordinates_file_name.c_str (), ios::in);
  if (node_coordinates_file.fail ())
    {
      NS_FATAL_ERROR ("File " << node_coordinates_file_name.c_str () << " not found");
    }
  vector<vector<double> > coord_array;
  int m = 0;

  while (!node_coordinates_file.eof ())
    {
      string line;
      getline (node_coordinates_file, line);

      if (line == "")
        {
          NS_LOG_WARN ("WARNING: Ignoring blank row: " << m);
          break;
        }

      istringstream iss (line);
      double coordinate;
      vector<double> row;
      int n = 0;
      while (iss >> coordinate)
        {
          row.push_back (coordinate);
          n++;
        }

      if (n != 2)
        {
          NS_LOG_ERROR ("ERROR: Number of elements at line#" << m << " is "  << n << " which is not equal to 2 for node coordinates file");
          exit (1);
        }

      else
        {
          coord_array.push_back (row);
        }
      m++;
    }
  node_coordinates_file.close ();
  return coord_array;

}

void printMatrix (const char* description, vector<vector<bool> > array)
{
  cout << "**** Start " << description << "********" << endl;
  for (size_t m = 0; m < array.size (); m++)
    {
      for (size_t n = 0; n < array[m].size (); n++)
        {
          cout << array[m][n] << ' ';
        }
      cout << endl;
    }
  cout << "**** End " << description << "********" << endl;

}

void printCoordinateArray (const char* description, vector<vector<double> > coord_array)
{
  cout << "**** Start " << description << "********" << endl;
  for (size_t m = 0; m < coord_array.size (); m++)
    {
      for (size_t n = 0; n < coord_array[m].size (); n++)
        {
          cout << coord_array[m][n] << ' ';
        }
      cout << endl;
    }
  cout << "**** End " << description << "********" << endl;

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

void SinkRxTrace(Ptr<const Packet> pkt, const Address &addr)
{
  double lastRxTime = Simulator::Now().GetSeconds();
	int index = (int)(lastRxTime / interval);
	bytesPeriod[index] += pkt->GetSize();
}

// ---------- End of Function Definitions ------------------------------------
