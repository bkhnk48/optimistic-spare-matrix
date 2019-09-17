/* -*- Mode:C++; c-file-style:"microsoft"; indent-tabs-mode:nil; -*- */

#include <cstdio>

#include "ns3/log.h"
#include "ns3/data-rate.h"
#include "ns3/uinteger.h"
#include "ns3/boolean.h"
#include "ns3/node.h"
#include "ns3/channel.h"
#include "ns3/ethernet-header.h"
#include "ns3/ethernet-trailer.h"
#include "ns3/pointer.h"
#include "ns3/object-factory.h"

#include "tocino-net-device.h"
#include "tocino-rx.h"
#include "tocino-tx.h"
#include "tocino-channel.h"
#include "tocino-flit-header.h"
#include "tocino-dimension-order-router.h"
#include "tocino-simple-arbiter.h"
#include "tocino-flit-id-tag.h"

NS_LOG_COMPONENT_DEFINE ("TocinoNetDevice");

#ifdef NS_LOG_APPEND_CONTEXT
#pragma push_macro("NS_LOG_APPEND_CONTEXT")
#undef NS_LOG_APPEND_CONTEXT
#define NS_LOG_APPEND_CONTEXT \
    { std::clog << "(" \
                << (int) m_address.GetX() << "," \
                << (int) m_address.GetY() << "," \
                << (int) m_address.GetZ() << ") "; }
#endif

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (TocinoNetDevice);

TypeId TocinoNetDevice::GetTypeId(void)
{
    static TypeId tid = TypeId( "ns3::TocinoNetDevice" )
        .SetParent<NetDevice>()
        .AddAttribute( "Ports", 
            "Number of ports on net device.",
            UintegerValue( TocinoNetDevice::DEFAULT_NPORTS ),
            MakeUintegerAccessor( &TocinoNetDevice::SetNPorts ),
            MakeUintegerChecker<uint32_t>() )
        .AddAttribute( "VirtualChannels", 
            "Number of virtual channels on each port.",
            UintegerValue( TocinoNetDevice::DEFAULT_NVCS ),
            MakeUintegerAccessor( &TocinoNetDevice::SetNVCs ),
            MakeUintegerChecker<uint32_t>() )
        .AddAttribute( "RouterType",
            "Name of type which implements routing algorithm.",
            TypeIdValue( TocinoDimensionOrderRouter::GetTypeId() ),
            MakeTypeIdAccessor( &TocinoNetDevice::m_routerTypeId ),
            MakeTypeIdChecker() )
        .AddAttribute( "ArbiterType",
            "Name of type which implements arbitration algorithm.",
            TypeIdValue( TocinoSimpleArbiter::GetTypeId() ),
            MakeTypeIdAccessor( &TocinoNetDevice::m_arbiterTypeId ),
            MakeTypeIdChecker() )
        .AddAttribute( "RoundRobinVCInject", 
            "Round-robin switch injection VC on each packets.",
            BooleanValue( false ),
            MakeBooleanAccessor( &TocinoNetDevice::m_roundRobinVCInject),
            MakeBooleanChecker() )
        .AddConstructor<TocinoNetDevice>();
    return tid;
}

TocinoNetDevice::TocinoNetDevice()
    : m_node( NULL )
    , m_ifIndex( 0 )
    , m_mtu( DEFAULT_MTU )
    , m_address( 0 )
    , m_outgoingFlitsMaxSize( 0 )
    , m_rxCallback( NULL )
    , m_promiscRxCallback( NULL )
    , m_nPorts( DEFAULT_NPORTS )
    , m_nVCs( DEFAULT_NVCS )
    , m_routerTypeId( TocinoDimensionOrderRouter::GetTypeId() )
    , m_arbiterTypeId( TocinoSimpleArbiter::GetTypeId() )
    , m_roundRobinVCInject( false )
    , m_packetCounter( 0 )
{}

void
TocinoNetDevice::Initialize()
{
    // size data structures
    m_outgoingFlits.resize( m_nVCs );
    m_incomingPackets.resize( m_nVCs, NULL );
    m_incomingSources.resize( m_nVCs );
    m_receivers.resize( m_nPorts );
    m_transmitters.resize( m_nPorts );

    // create receivers and routers
    // create transmitters and arbiters
    for( uint32_t i = 0; i < m_nPorts; i++ )
    {
        // MAS - must create transmitter first
        m_transmitters[i] = new TocinoTx( i, this );
        m_receivers[i] = new TocinoRx( i, this );
    }
}

TocinoNetDevice::~TocinoNetDevice()
{}

void
TocinoNetDevice::DoDispose()
{
    NS_ASSERT( AllQuiet() );
   
    for (unsigned i = 0; i < m_receivers.size(); i++)
    {
        delete m_receivers[i];
    }

    for (unsigned i = 0; i < m_transmitters.size(); i++)
    {
        delete m_transmitters[i];
    }
}


void TocinoNetDevice::SetIfIndex( const uint32_t index )
{
    m_ifIndex = index;
}

uint32_t TocinoNetDevice::GetIfIndex( void ) const
{ 
    return m_ifIndex;
}

Ptr<Channel> TocinoNetDevice::GetChannel( void ) const
{
    // Tocino net devices have many channels,
    // so this API is not implemented.  See
    // the GetChannel(uint) version.
    
    NS_ASSERT( false );
    return 0;
}

bool TocinoNetDevice::SetMtu( const uint16_t mtu )
{
    m_mtu = mtu;
    return true;
}

uint16_t TocinoNetDevice::GetMtu( void ) const
{
    return m_mtu;
}

void TocinoNetDevice::SetAddress( Address address )
{
    m_address = TocinoAddress::ConvertFrom( address );
}

Address TocinoNetDevice::GetAddress( void ) const
{
    return m_address;
}

TocinoAddress TocinoNetDevice::GetTocinoAddress( void ) const
{
    return m_address;
}

bool TocinoNetDevice::IsLinkUp( void ) const
{
    return true;
}

void TocinoNetDevice::AddLinkChangeCallback( Callback<void> callback )
{
    //Do nothing for now 
}

bool TocinoNetDevice::IsBroadcast( void ) const
{
    return true;
}

Address TocinoNetDevice::GetBroadcast( void ) const
{
    return TocinoAddress::ConvertFrom( Mac48Address ("ff:ff:ff:ff:ff:ff") );
}

bool TocinoNetDevice::IsMulticast( void ) const
{
    return true;
}

Address TocinoNetDevice::GetMulticast( Ipv4Address a ) const
{
    return TocinoAddress::ConvertFrom( Mac48Address::GetMulticast( a ) );
}

Address TocinoNetDevice::GetMulticast( Ipv6Address a ) const
{
    return TocinoAddress::ConvertFrom( Mac48Address::GetMulticast( a ) );
}

bool TocinoNetDevice::IsPointToPoint( void ) const
{
    return false;
}

bool TocinoNetDevice::IsBridge( void ) const
{
    return false;
}

TocinoFlittizedPacket
TocinoNetDevice::Flitter( const Ptr<Packet> p, const TocinoAddress& src, const TocinoAddress& dst, const TocinoInputVC vc, const TocinoFlitHeader::Type type )
{
    uint32_t start = 0;
    bool isFirstFlit = true;
    
    TocinoFlittizedPacket q;
   
    uint32_t remainder = p->GetSize(); // GetSerializedSize?

    do
    {
        Ptr<Packet> flit;
        
        const uint32_t LIMIT = isFirstFlit ? 
            TocinoFlitHeader::MAX_PAYLOAD_HEAD :
            TocinoFlitHeader::MAX_PAYLOAD_OTHER;
        
        const bool isLastFlit = (remainder <= LIMIT);
       
        const uint32_t LEN = isLastFlit ? remainder : LIMIT;
            
        flit = p->CreateFragment( start, LEN );
    
        TocinoFlitHeader h( src, dst );

        if( isFirstFlit )
        {
            h.SetHead();
            h.SetType( type );
        }
        
        if( isLastFlit )
        {
            h.SetTail();
        }

        h.SetLength( LEN );

        h.SetVirtualChannel( vc );

        flit->AddHeader(h);
            
        q.push_back( flit );

        if( isFirstFlit )
        {
            isFirstFlit = 0;
        }
    
        start += LEN;
        remainder -= LEN;
    }
    while( remainder > 0 );

    NS_ASSERT_MSG( q.size() > 0, "Flitter must always produce at least one flit" );

    const uint32_t ABS_PACKET_NUM = TocinoFlitIdTag::NextPacketNumber();
    const uint32_t TOTAL_FLITS = q.size();

    for( uint32_t i = 0; i < TOTAL_FLITS; ++i )
    {
        const uint32_t REL_FLIT_NUM = i+1;

        TocinoFlitIdTag tag( ABS_PACKET_NUM, REL_FLIT_NUM, TOTAL_FLITS );
        q[i]->AddPacketTag( tag );
    }

    return q;
}

bool 
TocinoNetDevice::SendEx(
        Ptr<Packet> packet,
        const TocinoAddress& src,
        const TocinoAddress& dest,
        const TocinoAddress& via,
        uint16_t protocolNumber )
{
    NS_LOG_FUNCTION( packet << src << dest << protocolNumber );

    // Avoid modifying the passed-in packet.
    Ptr<Packet> p = packet->Copy();

    EthernetHeader eh( false );
    EthernetTrailer et;

    eh.SetSource( src.AsMac48Address() );
    eh.SetDestination( dest.AsMac48Address() );
    eh.SetLengthType( protocolNumber );

    p->AddHeader( eh );
    p->AddTrailer( et );

    uint32_t injectionVC = TOCINO_INVALID_VC;

    if( m_roundRobinVCInject )
    {
        // Round-robin across all the VC pairs
        //
        // N.B.
        // We may inject on the base (even-numbered)
        // VC of each pair. The high (odd-numbered) 
        // VCs are reserved for deadlock avoidance via
        // dateline algorithm.
        injectionVC = (m_packetCounter*2) % m_nVCs;
        m_packetCounter++;
    }
    else
    {
        // Always inject on VC 0
        injectionVC = 0;
    }

    TocinoFlittizedPacket fp
        = Flitter( p, src, dest, injectionVC, TocinoFlitHeader::ETHERNET );

    if( via.IsValid() )
    {
        TocinoAddIntermediateDestination( fp, via );
    }

    NS_ASSERT( injectionVC < m_outgoingFlits.size() );

    // add the new flits to the end of the outgoing flit Q
    // ISSUE-REVIEW: this can grow unbounded?
    m_outgoingFlits[injectionVC].insert(
            m_outgoingFlits[injectionVC].end(), fp.begin(), fp.end() );

    for( uint32_t vc = 0; vc < m_nVCs; ++vc )
    {
        //NS_ASSERT_MSG( m_outgoingFlits[vc].size() < 100, 
        //      "Crazy large packet queue?" );

        m_outgoingFlitsMaxSize =
            std::max( m_outgoingFlitsMaxSize, m_outgoingFlits[vc].size() );
    }

    TrySendFlits();

    // Tocino is a lossless network
    return true;
}

bool
TocinoNetDevice::Send( 
        Ptr<Packet> packet,
        const Address& dest,
        uint16_t protocolNumber )
{
    return SendEx(
            packet, 
            m_address,
            TocinoAddress::ConvertFrom(dest),
            TocinoAddress(),
            protocolNumber );
}

bool 
TocinoNetDevice::SendFrom(
        Ptr<Packet> packet,
        const Address& src,
        const Address& dest,
        uint16_t protocolNumber )
{
    return SendEx(
            packet, 
            TocinoAddress::ConvertFrom(src),
            TocinoAddress::ConvertFrom(dest),
            TocinoAddress(),
            protocolNumber );
}

bool
TocinoNetDevice::SendVia(
        Ptr<Packet> packet,
        const Address& dest,
        const Address& via,
        uint16_t protocolNumber )
{
    return SendEx(
            packet, 
            m_address,
            TocinoAddress::ConvertFrom(dest),
            TocinoAddress::ConvertFrom(via),
            protocolNumber );
}

Ptr<Node>
TocinoNetDevice::GetNode( void ) const {return m_node;}

void
TocinoNetDevice::SetNode( Ptr<Node> node ) {m_node = node;}

bool
TocinoNetDevice::NeedsArp( void ) const {return true;}

void
TocinoNetDevice::SetReceiveCallback( NetDevice::ReceiveCallback cb )
{
    m_rxCallback = cb;
}

void
TocinoNetDevice::SetPromiscReceiveCallback( PromiscReceiveCallback cb )
{
    m_promiscRxCallback = cb;
}

bool TocinoNetDevice::SupportsSendFrom( void ) const
{
    return true;
}

void
TocinoNetDevice::SetChannel(
        uint32_t port,
        Ptr<TocinoChannel> chan )
{
    NS_ASSERT( port < m_receivers.size() );
    NS_ASSERT( port < m_transmitters.size() );

    m_receivers[port]->SetChannel( chan );
    m_transmitters[port]->SetChannel( chan );
}

Ptr<TocinoChannel>
TocinoNetDevice::GetChannel(
        uint32_t port )
{
    NS_ASSERT( port < m_transmitters.size() );

    return m_transmitters[port]->GetChannel();
}

void TocinoNetDevice::InjectFlit( Ptr<Packet> f ) const
{
    const bool head = IsTocinoFlitHead( f );
    const bool tail = IsTocinoFlitTail( f );

    if( head && tail )  
    {
        NS_LOG_LOGIC( GetTocinoFlitIdString( f ) << " singleton" );
    }
    else if( head ) 
    {
        NS_LOG_LOGIC( GetTocinoFlitIdString( f ) << " head" );
    }
    else if( tail )
    {
        NS_LOG_LOGIC( GetTocinoFlitIdString( f ) << " tail" );
    }
    else
    {
        NS_ASSERT( !head && !tail );
        NS_LOG_LOGIC( GetTocinoFlitIdString( f ) << " body" );
    }

    m_receivers[ GetHostPort() ]->Receive(f);
}

void TocinoNetDevice::TrySendFlits()
{
    NS_LOG_FUNCTION_NOARGS();
   
    for( uint32_t vc = 0; vc < m_nVCs; ++vc )
    {
        if( m_outgoingFlits[vc].empty() )
        {
            continue;
        }

        while( !m_outgoingFlits[vc].empty() )
        {
            Ptr<Packet> flit = m_outgoingFlits[vc].front();

            NS_ASSERT( vc == GetTocinoFlitVirtualChannel( flit ) );

            if( m_receivers[ GetHostPort() ]->IsVCBlocked( vc ) )
            {
                break;
            }

            // must pop prior to calling InjectFlit; InjectFlit can indirectly generate
            // a call to TrySendFlits which can cause a flit to be sent twice if pop
            // occurs after InjectFlits
            m_outgoingFlits[vc].pop_front();

            InjectFlit(flit);
        }
    }
}

void TocinoNetDevice::EjectFlit( Ptr<Packet> f )
{
    NS_LOG_FUNCTION( GetTocinoFlitIdString( f ) );
    
    TocinoFlitHeader h;
    f->RemoveHeader( h );
  
    NS_ASSERT( m_incomingPackets.size() == m_nVCs );
    NS_ASSERT( m_incomingSources.size() == m_nVCs );

    const uint32_t vc = h.GetVirtualChannel().AsUInt32();

    Ptr< Packet >& pkt = m_incomingPackets[vc];
    TocinoAddress& src = m_incomingSources[vc];

    if( pkt == NULL )
    {
        NS_ASSERT_MSG( h.IsHead(), "First flit must be head flit" );
        NS_ASSERT_MSG( h.GetDestination() == m_address,
            "Ejected packet for foreign address?" );

        NS_ASSERT_MSG( h.GetType() == TocinoFlitHeader::ETHERNET,
            "Ejected packet type is not ethernet?" );
        
        pkt = f;
        src = h.GetSource();
    }
    else
    {
        NS_ASSERT( !h.IsHead() );
        pkt->AddAtEnd( f );
    }
        
    NS_ASSERT( pkt != NULL );

    if( h.IsTail() )
    {
        EthernetHeader eh;
        EthernetTrailer et;

        pkt->RemoveHeader( eh );
        pkt->RemoveTrailer( et );

        NS_ASSERT_MSG( eh.GetSource() == src.AsMac48Address(),
            "Encapsulated Ethernet frame has a difference source than head flit" );
        
        NS_ASSERT_MSG( eh.GetDestination() == m_address.AsMac48Address(),
            "Encapsulated Ethernet frame has a foreign destination address?" );

        TocinoFlitIdTag tag;
        bool success = pkt->RemovePacketTag( tag );
        NS_ASSERT_MSG( success == true, "Expected TocinoFlitIdTag" );

        m_rxCallback( this, pkt, eh.GetLengthType(), src );
        pkt = NULL;
    }
}

const TypeId&
TocinoNetDevice::GetRouterTypeId() const
{
    return m_routerTypeId;
}

const TypeId&
TocinoNetDevice::GetArbiterTypeId() const
{
    return m_arbiterTypeId;
}

TocinoRx*
TocinoNetDevice::GetReceiver(
        const TocinoInputPort inputPort ) const
{
    NS_ASSERT( inputPort < m_receivers.size() );
    return m_receivers[ inputPort.AsUInt32() ];
}

TocinoTx*
TocinoNetDevice::GetTransmitter(
        const TocinoOutputPort outputPort ) const
{
    NS_ASSERT( outputPort < m_receivers.size() );
    return m_transmitters[ outputPort.AsUInt32() ];
}

uint32_t
TocinoNetDevice::GetNPorts() const
{ 
    return m_nPorts;
}

void
TocinoNetDevice::SetNPorts( uint32_t ports )
{ 
    NS_ASSERT( ports <= TOCINO_MAX_PORTS );
    
    ports = std::min( ports, TOCINO_MAX_PORTS );
    m_nPorts = ports;
}

uint32_t
TocinoNetDevice::GetNVCs() const
{
    return m_nVCs;
}

void
TocinoNetDevice::SetNVCs( uint32_t vcs )
{ 
    NS_ASSERT( vcs <= TOCINO_MAX_VCS );
    
    vcs = std::min( vcs, TOCINO_MAX_VCS );
    m_nVCs = vcs;
}

uint32_t
TocinoNetDevice::GetHostPort() const
{
    return m_nPorts - 1;
}

bool TocinoNetDevice::AllQuiet() const
{
    bool quiet = true;

    for( uint32_t vc = 0; vc < m_nVCs; ++vc )
    {
        if( !m_outgoingFlits[vc].empty() )
        {
            NS_LOG_LOGIC( "Not quiet: TrySendFlits() in progress?" );
            quiet = false;
        }
    }

    for (unsigned i = 0; i < m_incomingPackets.size(); i++)
    {
        if( m_incomingPackets[i] != NULL )
        {
            NS_LOG_LOGIC( "Not quiet: EjectFlits() in progress?" );
            quiet = false;
        }
    }

    for (unsigned i = 0; i < m_receivers.size(); i++)
    {
        if( !m_receivers[i]->AllQuiet() )
        {
            quiet = false;
        }
    }

    for (unsigned i = 0; i < m_transmitters.size(); i++)
    {
        if( !m_transmitters[i]->AllQuiet() )
        {
            quiet = false;
        }
    }
	
    return quiet;
}

void
TocinoNetDevice::DumpState() const
{
    for (unsigned i = 0; i < m_nPorts; i++)
    {
        m_transmitters[i]->DumpState();
    }

    for (unsigned i = 0; i < m_nPorts; i++)
    {
        m_receivers[i]->DumpState();
    }
}

void
TocinoNetDevice::ReportStatistics() const
{
    NS_LOG_LOGIC( "m_outgoingFlits reached "
            << m_outgoingFlitsMaxSize
            << " entries" );

    for (unsigned i = 0; i < m_transmitters.size(); i++)
    {
        m_transmitters[i]->ReportStatistics();
    }
}

#pragma pop_macro("NS_LOG_APPEND_CONTEXT")

} // namespace ns3
