/// This is a internal header
#if !defined(ZINX_PACKET_H)
#define ZINX_PACKET_H

#include <muduo/base/Endian.h>
#include <zinx/inc/Packet.h>
#include <cstdint>

namespace zinx {

/**
 * @code
 *                          ZinxPacket
 * +---------------------+-----------------------+---------------------+
 * |  ID (uint32_t)      | DataLength (uint32_t) |      Payload        |
 * +---------------------+-----------------------+---------------------+
 * 0                     4                       8                    size
 * @endcond
 */

/// ZinxPacket is inherited from Packet, to implement the TLD packet
class ZinxPacket : public Packet {
    
    static const uint32_t kHeaderLength = 8;

public:
    using Packet::Packet;   // use Packet`s constructor
    
    /// constrcut a ZinxPacket with specific id and payload
    ZinxPacket(uint32_t id, const std::string& payload);

    virtual uint32_t GetHeaderLength() const
    { return kHeaderLength; }
    
    virtual uint32_t GetPayloadLength() const
    {
        uint32_t len_be = * (static_cast<const uint32_t*>(static_cast<const void*>(data_.data() + sizeof(uint32_t))));
        return muduo::base::endian::BigToNative(len_be);
    }

    virtual ~ZinxPacket() = default;

    uint32_t GetId() const
    {
        uint32_t id_be = * (static_cast<const uint32_t*>(static_cast<const void*>(data_.data())));
        return muduo::base::endian::BigToNative(id_be);
    }
};

} // namespace zinx 

#endif // ZINX_PACKET_H
