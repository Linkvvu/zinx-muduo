#include <zinx/inc/ZPacket.h>
#include <zinx/inc/ZPacket_LTD.h>
#include <mmo_game/api/msg.pb.h>
#include <mmo_game/api/msg_id.h>
#include <mmo_game/core/Player.h>
#include <atomic>

using namespace mmo;

/// use atomic type to avoid race-condition
static std::atomic_int32_t global_id(std::numeric_limits<int32_t>::min());

Player::Player(int32_t pid, const zinx::ZinxConnectionPtr& conn, const Position& pos)
    : pid_(pid)
    , conn_(conn)
    , pos_(pos)
    { }

void Player::SendMsgWithProtobuf(uint32_t id, google::protobuf::Message* msg) {
    /* assert that connection is exist */
    assert(conn_.operator bool());
    
    /// TODO: Reuse the string held by each player to store encoded messages to reduce heap fragmentation
    std::string encode_msg = msg->SerializeAsString();
    assert(!encode_msg.empty());
    
    /* pack and send */
    zinx::ZinxPacket_LTD z_pack(id, encode_msg);
    conn_->Send(z_pack.GetAllData());
}

void Player::SyncPid() {
    mmo::pb::SyncPid syncPid_packet;
    syncPid_packet.set_pid(pid_);
    SendMsgWithProtobuf(SYNC_PID_Protocol_ID, &syncPid_packet);
}


void mmo::Player::BroadcastInitialPos() {
    mmo::pb::BroadCast broadcast_packet;
    broadcast_packet.set_tp(BroadCast_Positioni);
    broadcast_packet.set_pid(pid_);

    broadcast_packet.mutable_p()->set_x(pos_.X);
    broadcast_packet.mutable_p()->set_y(pos_.Y);
    broadcast_packet.mutable_p()->set_z(pos_.Z);
    broadcast_packet.mutable_p()->set_v(pos_.V);
    SendMsgWithProtobuf(BroadCast_Protocol_ID, &broadcast_packet);
}

std::shared_ptr<Player> mmo::CreateNewPlayer(const zinx::ZinxConnectionPtr& conn, const Position& pos) {
    std::shared_ptr<Player> p;
    p.reset(new Player(global_id++, conn, pos));
    return p;
}
