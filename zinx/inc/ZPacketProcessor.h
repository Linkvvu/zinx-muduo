#if !defined(MUDUO_ZINX_PACKET_PROCESSOR_H)
#define MUDUO_ZINX_PACKET_PROCESSOR_H
#include <zinx/inc/PacketProcessor.h>
#include <zinx/inc/ZPacket.h>
#include <memory>
namespace zinx {

class ZinxPacketProcessor : public PacketProcessor {
public:
    virtual bool Unpcak(muduo::Buffer* input_buf, Packet* result) const override;
};

} // namespace zinx 

#endif // MUDUO_ZINX_PACKET_PROCESSOR_H
