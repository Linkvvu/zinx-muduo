#include <zinx/inc/ZPacketProcessor_LTD.h>
#include <zinx/inc/ZPacket_LTD.h>
#include <cassert>

using namespace zinx;

bool ZinxPacketProcessor_LTD::Unpcak(muduo::Buffer* input_buf, Packet* result) const {
    // check when debug
    assert(dynamic_cast<ZinxPacket_LTD*>(result) != nullptr);

    /* tip: ZinxPacket`s header length is fixed value, so calling 'Packet::GetHeaderLength()' in here is fine */
    if (input_buf->ReadableBytes() < result->GetHeaderLength()) {
        return false;
    }

    /* get data length */
    const char* len_pos = input_buf->Peek();
    uint32_t data_len_le = *(static_cast<const uint32_t*>(static_cast<const void*>(len_pos)));
    uint32_t data_length = muduo::base::endian::LittleToNative(data_len_le);
    if (input_buf->ReadableBytes() < result->GetHeaderLength() + data_length) {
        return false;
    }

    std::string msg = input_buf->RetrieveAsString(result->GetHeaderLength() + data_length);
    result->Reset(std::move(msg));

    return true;
}