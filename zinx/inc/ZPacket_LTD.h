/// This file implement a packet that is LTD(length-tag-data) format, 
/// to be compatible with MMO-Game client

/**
 * @code
 *                          ZinxPacket
 * +------------------------+-----------------+---------------------+
 * |  DataLength (uint32_t) |  ID (uint32_t)  |      Payload        |
 * |     little endian      |  little endian  |                     |
 * +------------------------+-----------------+---------------------+
 * 0                     4                       8                    size
 * @endcode
 */

#if !defined(ZINX_PACKET_LTD_H)
#define ZINX_PACKET_LTD_H

#include <muduo/base/Endian.h>
#include <zinx/inc/Packet.h>
#include <cstdint>
#include <cassert>

namespace zinx {

/// ZinxPacket is inherited from Packet, to implement the LTD packet
class ZinxPacket_LTD : public base::Packet {
    
    static const uint32_t kHeaderLength = 8;

public:
    using Packet::Packet;   // use Packet`s constructor
    
    /// construct a ZinxPacket_LTD with specific id and payload
    ZinxPacket_LTD(uint32_t id, const std::string& payload);

    virtual uint32_t GetHeaderLength() const
    { return kHeaderLength; }
    
    virtual uint32_t GetPayloadLength() const
    {
        assert(!data_.empty());
        uint32_t len_le = * (static_cast<const uint32_t*>(static_cast<const void*>(data_.data())));
        return muduo::base::endian::LittleToNative(len_le);
    }

    virtual ~ZinxPacket_LTD() = default;

    uint32_t GetId() const
    {
        assert(!data_.empty());
        uint32_t id_le = * (static_cast<const uint32_t*>(static_cast<const void*>(data_.data() + sizeof(uint32_t))));
        return muduo::base::endian::LittleToNative(id_le);
    }
};

} // namespace zinx 

#endif // ZINX_PACKET_LTD_H
