#include <muduo/base/Logging.h>
#include <mmo_game/util/util.h>
#include <random>

zinx::ZinxPacket_LTD mmo::util::packToLTDWithProtobuf(uint32_t id, google::protobuf::Message* msg) {
    std::string encoded_msg = msg->SerializeAsString();
    // assert that serializeation is successful
    assert(!encoded_msg.empty());

    zinx::ZinxPacket_LTD packet(id, std::move(encoded_msg));
    return packet;
}

/// TODO: Accept boundary values to dynamically adjust the range of random numbers
mmo::Position mmo::util::getRandomPoistion() {
    std::default_random_engine rand_engine;
    std::uniform_int_distribution<int> random(0, 30);
    rand_engine.seed(time(nullptr));

    return mmo::Position {
        160+random(rand_engine),    // length
        0,                          // height
        240+random(rand_engine),    // width
        0,                          // angle
    };
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
int32_t mmo::util::getPidFromZConnection(const zinx::ZinxConnectionPtr& ctx) {
    try {
        return std::any_cast<int32_t>(ctx->GetContext());
    } catch (const std::bad_any_cast& e) {
    /// FIXME: Disconnect the current connection instead of abort
        /// If failed to get pid, @c abort()
        LOG_FATAL << e.what(); 
    }
}
#pragma GCC diagnostic pop
