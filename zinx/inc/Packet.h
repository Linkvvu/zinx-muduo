#if !defined(ZINX_INTERFACE_PACKET_H)
#define ZINX_INTERFACE_PACKET_H

#include <zinx/inc/Copyable.h>
#include <cstdint>
#include <string>
#include <memory>
#include <algorithm>

namespace zinx {
/// The class is a abstract base class for packet
class Packet : Copyable {
public:
    /// construct a empty packet by default
    Packet(const std::string& data = std::string())
        : data_(data)
    {
        // Note: Can`t use polymorphic (virtual) method in constructor and destructor!!!
    }

    virtual ~Packet() noexcept = default;

    // should abstract to zinx::ZinxPacket as it normal member function
    // virtual uint32_t GetId() const = 0;

    virtual uint32_t GetPayloadLength() const = 0; 

    virtual uint32_t GetHeaderLength() const = 0;

    const std::string& GetAllData() const
    { return data_; }

    std::string& GetAllData()
    { return data_; }

    const char* GetPayload() const 
    { return data_.data() + GetHeaderLength(); }

    char* GetPayload() 
    { return data_.data() + GetHeaderLength(); }

    void Reset(const std::string& data) {
        data_ = data;
    }

    void Reset(std::string&& data) {
        data_ = std::move(data);
    }

protected:
    std::string data_;
};

using PacketPtr = std::shared_ptr<Packet>;

} // namespace zinx 


#endif // ZINX_INTERFACE_PACKET_H
