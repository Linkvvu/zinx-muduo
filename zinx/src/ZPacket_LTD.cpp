#include <zinx/inc/ZPacket_LTD.h>

using namespace zinx;

ZinxPacket_LTD::ZinxPacket_LTD(uint32_t id, const std::string& payload) {
    /// FIXME: check payload boundary
    data_.reserve(ZinxPacket_LTD::kHeaderLength + payload.size());

    // uint32_t len_be = muduo::base::endian::NativeToBig(static_cast<uint32_t>(payload.size()));
    // uint32_t id_be = muduo::base::endian::NativeToBig(id);

    uint32_t len_le = muduo::base::endian::NativeToLittle(static_cast<uint32_t>(payload.size()));
    uint32_t id_le = muduo::base::endian::NativeToLittle(id);

    data_.append(static_cast<char*>(static_cast<void*>(&len_le)), sizeof (uint32_t));
    data_.append(static_cast<char*>(static_cast<void*>(&id_le)), sizeof (uint32_t));
    data_ += payload;
}
