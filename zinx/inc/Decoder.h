#if !defined(MUDUO_INTERFACE_DECODER_H)
#define MUDUO_INTERFACE_DECODER_H

#include <muduo/Buffer.h>
#include <zinx/inc/Packet.h>
#include <memory>

namespace zinx {
namespace base {

class Decoder {
public:
    /// @brief Decode a complete packet
    /// @param input_buf connection`s input data
    /// @return if a complete packet is received, return the @c PacketPtr; otherwise, return @c std::shared_ptr<T>(nullptr)
    virtual PacketPtr Decode(muduo::Buffer* input_buf) const = 0;

    /// for polymorphism of abstract base class
    virtual ~Decoder() noexcept = default;
};

} // namespace base 
} // namespace zinx 

#endif // MUDUO_INTERFACE_DECODER_H
