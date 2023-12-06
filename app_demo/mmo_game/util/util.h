#if !defined(MMO_UTIL_UTIL_H)
#define MMO_UTIL_UTIL_H

#include <google/protobuf/message.h>
#include <zinx/inc/RequestContext.h>
#include <zinx/inc/ZPacket_LTD.h>
#include <mmo_game/core/Player.h>

namespace mmo {
namespace util {

    /// Pack a LTD-packet With Protobuf 
    extern zinx::ZinxPacket_LTD packToLTDWithProtobuf(uint32_t id, google::protobuf::Message* msg);
    extern mmo::Position getRandomPoistion();
    extern int32_t getPidFromZConnection(const zinx::ZinxConnectionPtr& ctx);

} // namespace util 
} // namespace mmo 

#endif // MMO_UTIL_UTIL_H
