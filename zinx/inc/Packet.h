#if !defined(ZINX_INTERFACE_PACKET_H)
#define ZINX_INTERFACE_PACKET_H

#include <zinx/inc/Copyable.h>
#include <cstdint>
#include <string>
#include <memory>
#include <algorithm>

namespace zinx {
namespace base {

/// The class is a abstract base class for packet
class Packet : Copyable {
public:
    /// construct a empty packet by default
    Packet()
        : data_(std::string())
    { }

    virtual ~Packet() noexcept = default;

    // should abstract to zinx::ZinxPacket as it normal member function
    // virtual uint32_t GetId() const = 0;

    virtual uint32_t GetPayloadLength() const = 0; 

    virtual uint32_t GetHeaderLength() const = 0;

    const std::string& GetData() const
    { return data_; }

    std::string& GetData()
    { return data_; }

    std::string GetPayload() 
    { return data_.substr(GetHeaderLength()); }

    void Reset(const std::string& data) {
        data_ = data;
    }

    void Reset(std::string&& data) {
        data_ = std::move(data);
    }

protected:
    std::string data_;
};
} // namespace base 


using PacketPtr = std::unique_ptr<base::Packet>;

} // namespace zinx 


#endif // ZINX_INTERFACE_PACKET_H
