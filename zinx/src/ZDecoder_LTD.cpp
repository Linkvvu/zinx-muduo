#include <zinx/inc/ZDecoder_LTD.h>
#include <zinx/inc/ZPacket_LTD.h>
#include <cassert>

using namespace zinx;

PacketPtr ZinxDecoder_LTD::Decode(muduo::Buffer* input_buf) const {
    PacketPtr pack = std::make_unique<ZinxPacket_LTD>();
    
    /* tip: ZinxPacket`s header length is fixed value, so calling 'Packet::GetHeaderLength()' in here is fine */
    if (input_buf->ReadableBytes() < pack->GetHeaderLength()) {
        return nullptr;
    }

    /* get data length */
    const char* len_pos = input_buf->Peek();
    uint32_t data_len_le = *(static_cast<const uint32_t*>(static_cast<const void*>(len_pos)));
    uint32_t data_length = muduo::base::endian::LittleToNative(data_len_le);
    if (input_buf->ReadableBytes() < pack->GetHeaderLength() + data_length) {
        return nullptr;
    }

    std::string msg = input_buf->RetrieveAsString(pack->GetHeaderLength() + data_length);
    pack->Reset(std::move(msg));

    return pack;
}