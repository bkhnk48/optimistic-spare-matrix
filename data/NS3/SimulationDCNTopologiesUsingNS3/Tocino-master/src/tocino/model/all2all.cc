/* -*- Mode:C++; c-file-style:"microsoft"; indent-tabs-mode:nil; -*- */

#include "ns3/log.h"
#include "ns3/node.h"
#include "ns3/packet.h"
#include "ns3/random-variable-stream.h"
#include "ns3/rng-seed-manager.h"
#include "ns3/simulator.h"

#include "tocino-flit-header.h"

#include "all2all.h"

using namespace std;

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("All2All");
NS_OBJECT_ENSURE_REGISTERED (All2All);

TypeId 
All2All::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::All2All")
      .SetParent<Application> ()
      .AddConstructor<All2All> ()
      .AddAttribute ("MTBS", "Mean time between Send operations, exponential distribution.",
          TimeValue (Seconds(0.001)),
          MakeTimeAccessor (&All2All::m_mtbs),
          MakeTimeChecker ())
      .AddAttribute ("MaxdT", "Max delay between Send operations.",
          TimeValue (Seconds(1)),
          MakeTimeAccessor (&All2All::m_maxdt),
          MakeTimeChecker ())
      .AddAttribute ("Size", "Send size in bytes.",
          UintegerValue (64),
          MakeUintegerAccessor (&All2All::m_size),
          MakeUintegerChecker<uint32_t>())
      ;
  return tid;
}

All2All::All2All() 
{
    m_destRV = CreateObject<UniformRandomVariable>();
    m_dtRV = CreateObject<ExponentialRandomVariable>();

    m_nPacketsIn = 0;
    m_nPacketsOut = 0;
}

All2All::~All2All() {}

void All2All::DoDispose(void)
{
    m_netDevices.clear();
    NS_LOG_UNCOND("NetDevice=" << m_myNetDevice->GetAddress()
        << " nPacketsOut=" << m_nPacketsOut 
        << " nPacketsIn=" << m_nPacketsIn);

  // chain up
  Application::DoDispose ();
}

void All2All::StartApplication ()
{
  NS_LOG_FUNCTION(this);
  NS_ASSERT(m_netDevices.size() > 0);

  Ptr<Node> node = GetNode();
  NS_ASSERT(node->GetNDevices() == 1);
  m_myNetDevice = node->GetDevice(0);
  m_myNetDevice->SetReceiveCallback(MakeCallback(&All2All::ReceiveFromDevice, this));

  NS_ASSERT(m_netDevices.size() > 0); // comm targets
  Send();
}

void All2All::StopApplication ()
{
  NS_LOG_FUNCTION(this);
  Simulator::Cancel(m_sendEvent);
}

void
All2All::AddRemote(Ptr<NetDevice> nd)
{
    m_netDevices.push_back(nd);
}

void
All2All::ScheduleTransmit(Time dt)
{
    NS_LOG_FUNCTION_NOARGS();
    m_sendEvent = Simulator::Schedule(dt, &All2All::Send, this);
}

void
All2All::Send(void)
{
    NS_LOG_FUNCTION_NOARGS();
    NS_ASSERT(m_sendEvent.IsExpired()); // simulation ended

    // create the packet
    Ptr<Packet> p = Create<Packet> (m_size);

    // randomly target packet - uniform over registered NetDevices
    uint32_t dest = m_destRV->GetInteger(0, m_netDevices.size()-1); // GetInteger includes endpoints
    Address da = m_netDevices[dest]->GetAddress();
    m_myNetDevice->Send(p, da, 0); // need a "raw" protocol type here
    m_nPacketsOut++;

    // compute delay to next send and schedule event
    Time dt = Time(m_dtRV->GetValue(m_mtbs.GetDouble(), m_maxdt.GetDouble()));
    ScheduleTransmit(dt);
}

void
All2All::Receive(void)
{
    NS_LOG_FUNCTION(this);
}

bool
All2All::ReceiveFromDevice(Ptr<NetDevice> device, Ptr<const Packet> p, uint16_t protocol, const Address& from)
{
    NS_LOG_FUNCTION(this);

    m_nPacketsIn++;
    return true;
}

} // namespace ns3
