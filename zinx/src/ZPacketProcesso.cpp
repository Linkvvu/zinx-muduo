#include <muduo/base/Endian.h>
#include <zinx/inc/ZPacketProcessor.h>
#include <cassert>

using namespace zinx;

bool ZinxPacketProcessor::Unpcak(muduo::Buffer* input_buf, Packet* result) const {
    // check when debug
    assert(dynamic_cast<ZinxPacket*>(result) != nullptr);

    /* tip: ZinxPacket`s header length is fixed value, so calling 'Packet::GetHeaderLength()' in here is fine */
    if (input_buf->ReadableBytes() < result->GetHeaderLength()) {
        return false;
    }

    /* get data length */
    const char* len_position = input_buf->Peek() + sizeof(uint32_t);
    uint32_t data_length_be = *(static_cast<const uint32_t*>(static_cast<const void*>(len_position)));
    uint32_t data_length = muduo::base::endian::BigToNative(data_length_be);
    if (input_buf->ReadableBytes() < result->GetHeaderLength() + data_length) {
        return false;
    }

    std::string msg = input_buf->RetrieveAsString(result->GetHeaderLength() + data_length);
    result->Reset(std::move(msg));

    return true;
}
