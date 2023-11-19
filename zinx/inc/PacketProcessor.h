#if !defined(MUDUO_INTERFACE_PACKET_PROCESSOR_H)
#define MUDUO_INTERFACE_PACKET_PROCESSOR_H

#include <muduo/Buffer.h>
#include <zinx/inc/Packet.h>
#include <memory>
#include <any>

namespace zinx {

class PacketProcessor {
public:
    /// @brief unpack a complete packet
    /// @param input_buf connection`s input datas
    /// @param result complete packet is returned, do nothing if the current packet is incomplete
    /// @return if a complete packet is received, return true; otherwise, return false
    virtual bool Unpcak(muduo::Buffer* input_buf, Packet* result) const = 0;

    /// for polymorphism of abstract base class
    virtual ~PacketProcessor() noexcept = default;
};

} // namespace zinx 

#endif // MUDUO_INTERFACE_PACKET_PROCESSOR_H
