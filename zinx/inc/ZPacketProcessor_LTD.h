/// This file implement a packet processor for LTD(length-tag-data) format, 
/// to process packets sent by the mmo client

#if !defined(ZINX_PACKET_PROCESSOR_LTD_H)
#define ZINX_PACKET_PROCESSOR_LTD_H
#include <zinx/inc/PacketProcessor.h>
namespace zinx {

class ZinxPacketProcessor_LTD : public zinx::PacketProcessor {
public:
    virtual bool Unpcak(muduo::Buffer* input_buf, Packet* result) const override;
    virtual ~ZinxPacketProcessor_LTD() noexcept = default;
};

} // namespace zinx 

#endif // ZINX_PACKET_PROCESSOR_LTD_H
