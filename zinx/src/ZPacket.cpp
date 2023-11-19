#include <zinx/inc/ZPacket.h>

using namespace zinx;

ZinxPacket::ZinxPacket(uint32_t id, const std::string& payload) {
    /// FIXME: check payload boundary
    data_.reserve(ZinxPacket::kHeaderLength + payload.size());

    uint32_t id_be = muduo::base::endian::NativeToBig(id);
    uint32_t len_be = muduo::base::endian::NativeToBig(static_cast<uint32_t>(payload.size()));

    data_.append(static_cast<char*>(static_cast<void*>(&id_be)), sizeof (uint32_t));
    data_.append(static_cast<char*>(static_cast<void*>(&len_be)), sizeof (uint32_t));
    data_ += payload;
}
