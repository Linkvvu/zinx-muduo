#include <zinx/inc/ZPacket.h>
#include <zinx/inc/ZPacket_LTD.h>
#include <mmo_game/api/msg.pb.h>
#include <mmo_game/api/msg_id.h>
#include <mmo_game/core/Player.h>
#include <mmo_game/core/WorldManager.h>
#include <vector>
#include <atomic>

using namespace mmo;

/// use atomic type to avoid race-condition
// static std::atomic_int32_t global_id(std::numeric_limits<int32_t>::min());
static std::atomic_int32_t global_id(1);

namespace mmo {

static void SetPosition(mmo::pb::Position* target, const mmo::Position& pos) {
    assert(target != nullptr);
    target->set_x(pos.X);
    target->set_y(pos.Y);
    target->set_z(pos.Z);
    target->set_v(pos.V);
}

} // namespace mmo 

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

std::vector<Player*> Player::GetSurroundingPlayers(const WorldManager& wm) const {
    std::vector<Player*> players;
    std::vector<int32_t> pids = wm.GetAoiManager().GetSurroundingPlayersByPid(pid_, pos_);
    players.reserve(pids.size());

    for (int32_t pid : pids) {
        players.emplace_back(wm.GetPlayerByPid(pid));
    }

    return players;
}

void Player::SyncPid() {
    mmo::pb::SyncPid syncPid_packet;
    syncPid_packet.set_pid(pid_);
    SendMsgWithProtobuf(Sync_Pid_Protocol_ID, &syncPid_packet);
}


void Player::SyncWithSurrounding(const mmo::WorldManager& wm) {
    /* get surrounding all players */
    std::vector<Player*> players = GetSurroundingPlayers(wm);

    mmo::pb::BroadCast broadcast_packet;
    broadcast_packet.set_pid(pid_);
    broadcast_packet.set_tp(BroadCast_Position);
    mmo::SetPosition(broadcast_packet.mutable_p(), pos_);

    std::vector<mmo::pb::Player> player_info_packets(players.size(), mmo::pb::Player());

    for (size_t i = 0; i < players.size(); i++) {
        Player* cur_player = players[i];
        /* pack a position-packet to broadcast to all surrounding players */
        cur_player->SendMsgWithProtobuf(BroadCast_Protocol_ID, &broadcast_packet);
        /* pack a surrounding player`s info-packet to send to current player so that sync info */
        player_info_packets[i].set_pid(cur_player->GetPid());
        mmo::SetPosition(player_info_packets[i].mutable_p(), cur_player->GetPosition());
    }

    /* tell current client that surrorunding players info(pid, position...) */
    mmo::pb::SyncPlayers sync_players_packet;
    sync_players_packet.mutable_ps()->Add(player_info_packets.begin(), player_info_packets.end());
    SendMsgWithProtobuf(Sync_With_Surrounding_Players_Protocol_ID, &sync_players_packet);
}

void Player::WorldChat(const std::string& content, const mmo::WorldManager& wm) {
    mmo::pb::BroadCast broadcast_packet;    
    broadcast_packet.set_pid(pid_);
    broadcast_packet.set_tp(BroadCast_World_Talk);
    broadcast_packet.set_content(content);

    std::vector<Player*> all_players = wm.GetAllPlayers();
    for (Player* p : all_players) {
        p->SendMsgWithProtobuf(BroadCast_Protocol_ID, &broadcast_packet);
    }
}

void Player::Offline(const mmo::WorldManager& wm) {
    const std::vector<Player*> surrounding_players = GetSurroundingPlayers(wm);
    mmo::pb::SyncPid sync_packet;
    sync_packet.set_pid(pid_);
    
    for (Player* p : surrounding_players) {
        p->SendMsgWithProtobuf(Sync_Disconnected_Player_Protocol_ID, &sync_packet);
    }
}

std::shared_ptr<Player> mmo::CreateNewPlayer(const zinx::ZinxConnectionPtr& conn, const Position& pos) {
    std::shared_ptr<Player> p;
    p.reset(new Player(global_id++, conn, pos));
    return p;
}
