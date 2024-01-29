#if !defined(ZINX_DECODER_H)
#define ZINX_DECODER_H

#include <zinx/inc/Decoder.h>
#include <memory>

namespace zinx {

class ZinxDecoder : public base::Decoder {
public:
    virtual PacketPtr Decode(muduo::Buffer* input_buf) const override;
    virtual ~ZinxDecoder() noexcept = default;
};

} // namespace zinx 

#endif // ZINX_DECODER_H
