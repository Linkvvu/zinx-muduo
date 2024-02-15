#if !defined(ZINX_INTERFACE_TAG_PACKET_H)
#define ZINX_INTERFACE_TAG_PACKET_H

#include <zinx/inc/Packet.h>

namespace zinx {
namespace base {

struct TagPacket : public Packet {
    TagPacket() = default;
    
    virtual ~TagPacket() noexcept = default;

    virtual uint32_t GetId() const = 0;

    virtual uint32_t GetPayloadLength() const = 0; 

    virtual uint32_t GetHeaderLength() const = 0;
};

} // namespace base 
} // namespace zinx 

#endif // ZINX_INTERFACE_TAG_PACKET_H
