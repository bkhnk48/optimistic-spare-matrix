/* -*- Mode:C++; c-file-style:"microsoft"; indent-tabs-mode:nil; -*- */
#ifndef __TOCINO_CHANNEL_H__
#define __TOCINO_CHANNEL_H__

#include "ns3/channel.h"
#include "ns3/nstime.h"
#include "ns3/data-rate.h"
#include "ns3/packet.h"

namespace ns3 
{

class TocinoNetDevice;
class TocinoTx;
class TocinoRx;

class TocinoChannel : public Channel
{
public:

    static TypeId GetTypeId();
    
    TocinoChannel();
    virtual ~TocinoChannel();
    
    bool TransmitStart(Ptr<Packet> p);
    Time GetTransmissionTime(Ptr<Packet> p);
    
    void SetTransmitter(TocinoTx* tx);
    void SetReceiver(TocinoRx* rx);
    uint32_t GetNDevices() const;
    
    static const uint32_t TX_DEV = 0;
    static const uint32_t RX_DEV = 1;
    
    Ptr<NetDevice> GetDevice(uint32_t i) const;
    Ptr<TocinoNetDevice> GetTocinoDevice(uint32_t i) const;

    uint32_t FlitBuffersRequired() const;
   
    uint32_t GetTotalBytesTransmitted() const;
    uint32_t GetTotalFlitsTransmitted() const;
    Time GetTotalTransmitTime() const;
    
    uint32_t GetLLCBytesTransmitted() const;
    uint32_t GetLLCFlitsTransmitted() const;
    Time GetLLCTransmitTime() const;
    
    void ReportStatistics() const;

private:
    
    void TransmitEnd ();
    
    // channel parameters
    Time m_delay;
    DataRate m_bps;
    
    Ptr<Packet> m_flit;
    
    TocinoTx* m_tx;
    TocinoRx* m_rx;
   
    std::string m_txString;
    std::string m_rxString;

    enum TocinoChannelState {IDLE, BUSY};
    TocinoChannelState m_state;

    uint32_t m_totalBytesTransmitted;
    uint32_t m_totalFlitsTransmitted;
    Time m_totalTransmitTime;
    
    uint32_t m_LLCBytesTransmitted;
    uint32_t m_LLCFlitsTransmitted;
    Time m_LLCTransmitTime;
    
    std::vector< uint32_t > m_vcUsageHistogram;
};

} // namespace ns3

#endif // __TOCINO_CHANNEL_H__
