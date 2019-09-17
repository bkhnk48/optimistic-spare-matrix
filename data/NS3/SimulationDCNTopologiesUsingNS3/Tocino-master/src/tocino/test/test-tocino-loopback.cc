/* -*- Mode:C++; c-file-style:"microsoft"; indent-tabs-mode:nil; -*- */

#include <stdint.h>
#include <vector>

#include "ns3/log.h"
#include "ns3/uinteger.h"
#include "ns3/packet.h"
#include "ns3/ptr.h"
#include "ns3/test.h"
#include "ns3/node.h"
#include "ns3/simulator.h"

#include "ns3/tocino-net-device.h"
#include "ns3/tocino-flit-header.h"
#include "ns3/tocino-misc.h"

#include "test-tocino-loopback.h"

using namespace ns3;

TestTocinoLoopback::TestTocinoLoopback()
  : TestCase( "Send flits from a single net device to itself" )
{
}

TestTocinoLoopback::~TestTocinoLoopback() {}

namespace
{
    unsigned totalCount;
    unsigned totalBytes;

    bool AcceptPacket( Ptr<NetDevice>, Ptr<const Packet> p, uint16_t, const Address& )
    {
        totalCount++;
        totalBytes += p->GetSize();
        
        return true;
    }
}

void TestTocinoLoopback::TestHelper( const unsigned COUNT, const unsigned BYTES )
{
    TocinoCustomizeLogging();

    Ptr<Packet> p = Create<Packet>( BYTES );
   
    Ptr<TocinoNetDevice> tnd = CreateObject<TocinoNetDevice>();
    
    tnd->Initialize();
    tnd->SetReceiveCallback( MakeCallback( AcceptPacket ) );
    tnd->SetAddress( TocinoAddress() );
    
    Ptr<Node> node = CreateObject<Node>();
    tnd->SetNode( node );

    totalCount = 0;
    totalBytes = 0;

    for( unsigned i = 0; i < COUNT; ++i )
    {
        // send to self
        Simulator::ScheduleWithContext( node->GetId(), Seconds(0),
            &TocinoNetDevice::Send, tnd, p, TocinoAddress(), 0 );
    }

    Simulator::Run();

    NS_TEST_ASSERT_MSG_EQ( totalCount, COUNT, "Got unexpected total packet count" );
    NS_TEST_ASSERT_MSG_EQ( totalBytes, BYTES*COUNT, "Got unexpected total packet bytes" );

    bool aq = tnd->AllQuiet();
    NS_TEST_ASSERT_MSG_EQ( aq, true, "Net device not quiet?" );
    
    Simulator::Destroy();
}

void
TestTocinoLoopback::DoRun (void)
{
    TestHelper( 1, 20 );
    TestHelper( 1, 123 );
    TestHelper( 2, 32 );
}
