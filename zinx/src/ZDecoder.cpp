#include <muduo/base/Endian.h>
#include <zinx/inc/ZDecoder.h>
#include <zinx/inc/ZPacket.h>

using namespace zinx;

PacketPtr ZinxDecoder::Decode(muduo::Buffer* input_buf) const {
    PacketPtr pack = std::make_unique<ZinxPacket>();

    /* tip: ZinxPacket`s header length is fixed value, so calling 'Packet::GetHeaderLength()' in here is fine */
    if (input_buf->ReadableBytes() < pack->GetHeaderLength()) {
        return nullptr;
    }

    /* get data length */
    const char* len_position = input_buf->Peek() + sizeof(uint32_t);
    uint32_t data_length_be = *(static_cast<const uint32_t*>(static_cast<const void*>(len_position)));
    uint32_t data_length = muduo::base::endian::BigToNative(data_length_be);
    if (input_buf->ReadableBytes() < pack->GetHeaderLength() + data_length) {
        return nullptr;
    }

    std::string msg = input_buf->RetrieveAsString(pack->GetHeaderLength() + data_length);
    pack->Reset(std::move(msg));

    return pack;
}
