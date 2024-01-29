/// This file implement a packet decoder for LTD(length-tag-data) format, 
/// to decode packets sent by the mmo client

#if !defined(ZINX_DECODER_LTD_H)
#define ZINX_DECODER_LTD_H
#include <zinx/inc/Decoder.h>
namespace zinx {

class ZinxDecoder_LTD : public base::Decoder {
public:
    virtual PacketPtr Decode(muduo::Buffer* input_buf) const override;
    virtual ~ZinxDecoder_LTD() noexcept = default;
};

} // namespace zinx 

#endif // ZINX_DECODER_LTD_H
