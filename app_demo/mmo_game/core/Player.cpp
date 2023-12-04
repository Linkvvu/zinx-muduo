#include <zinx/inc/ZConnection.h>
#include <mmo_game/api/msg_id.h>
#include <mmo_game/core/Player.h>
#include <mmo_game/api/msg.pb.h>
#include <mmo_game/util/util.h>
#include <mmo_game/core/WorldManager.h>
#include <atomic>

using namespace mmo;

/// Use atomic type to avoid race-condition
static std::atomic_int32_t global_player_id(1);

namespace mmo {

/// Yo set pb::Position by mmo::Position
static void SetPosition(mmo::pb::Position* target, const mmo::Position& pos) {
    assert(target != nullptr);
    target->set_x(static_cast<float>(pos.X));
    target->set_y(static_cast<float>(pos.Y));
    target->set_z(static_cast<float>(pos.Z));
    target->set_v(static_cast<float>(pos.V));
}

} // namespace mmo 

Player::Player(int32_t pid, const zinx::ZinxConnectionPtr& conn, const Position& pos)
    : pid_(pid)
    , conn_(conn)
    , pos_(pos)
    { }

std::shared_ptr<Player> mmo::CreateNewPlayer(const zinx::ZinxConnectionPtr& conn, const Position& pos) {
    std::shared_ptr<Player> p;
    p.reset(new Player(global_player_id++, conn, pos));
    return p;
}

void Player::SyncPid() {
    mmo::pb::SyncPid syncPid_packet;
    syncPid_packet.set_pid(pid_);
    const zinx::ZinxPacket_LTD& packet = util::packToLTDWithProtobuf(SYNC_PID_PACK_ID, &syncPid_packet);
    SendPacket(packet);
}


void mmo::Player::SyncWithSurrounding(const mmo::WorldManager& wm) {
    std::vector<Player*> surrounding_players = wm.GetSurroundingPlayers(pid_);
    mmo::pb::BroadCast broadcast_packet;
    broadcast_packet.set_pid(GetPid());
    broadcast_packet.set_tp(BC_POS_FIELD);
    mmo::SetPosition(broadcast_packet.mutable_p(), GetPosition());

    std::vector<mmo::pb::Player> player_info_packets(surrounding_players.size(), mmo::pb::Player());
    for (size_t i = 0; i < surrounding_players.size(); i++) {
        Player* cur_player = surrounding_players[i];
        // broadcast current player`s position to his surrounding players
        const zinx::ZinxPacket_LTD& packet = util::packToLTDWithProtobuf(BROADCAST_PACK_ID, &broadcast_packet);
        cur_player->SendPacket(packet);

        // prepare all the surrounding players info of current player to sync to the current client
        mmo::pb::Player cur_player_info;
        cur_player_info.set_pid(cur_player->GetPid());
        mmo::SetPosition(cur_player_info.mutable_p(), cur_player->GetPosition());
        player_info_packets[i] = cur_player_info;
    }

    mmo::pb::SyncPlayers sync_players_packet;
    sync_players_packet.mutable_ps()->Add(player_info_packets.begin(), player_info_packets.end());
    const zinx::ZinxPacket_LTD& packet = util::packToLTDWithProtobuf(SYNC_PLAYERS_PACK_ID, &sync_players_packet);
    SendPacket(packet);
}
