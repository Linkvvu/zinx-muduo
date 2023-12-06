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
    // std::memory_order_relaxed: Doesn`t need to sync, just need atomic operation 
    p.reset(new Player(global_player_id.fetch_add(1, std::memory_order_relaxed), conn, pos));
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

void mmo::Player::Disappear(const mmo::WorldManager& wm) {
    const std::vector<Player*> surrounding_players = wm.GetSurroundingPlayers(pid_);
    mmo::pb::SyncPid pb_sync_packet;
    pb_sync_packet.set_pid(pid_);
    const zinx::ZinxPacket_LTD& packet = util::packToLTDWithProtobuf(SYNC_LEAVE_PACK_ID, &pb_sync_packet);

    for (Player* p : surrounding_players) {
        p->SendPacket(packet);
    }
}

void mmo::Player::WorldChat(const std::string& content, const mmo::WorldManager& wm) {
    const std::vector<Player*> all_players = wm.GetAllPlayers();
    mmo::pb::BroadCast pb_bc_talk_packet;
    pb_bc_talk_packet.set_pid(pid_);
    pb_bc_talk_packet.set_tp(BC_TALK_FIELD);
    pb_bc_talk_packet.mutable_content()->assign(content);
    const zinx::ZinxPacket_LTD& packet = util::packToLTDWithProtobuf(BROADCAST_PACK_ID, &pb_bc_talk_packet);

    for (Player* p : all_players) {
        p->SendPacket(packet);
    }
}

void mmo::Player::UpdatePos(const Position& new_pos, const mmo::WorldManager& wm) {
    const Position old_pos = GetPosition();
    pos_ = new_pos;

    Grid& old_gird = wm.GetAoiManager().GetGridByPosition(old_pos);
    Grid& new_gird = wm.GetAoiManager().GetGridByPosition(new_pos);

    // Check whether the grid is crossed
    if (old_gird != new_gird) {
        old_gird.RemovePlayer(pid_);
        new_gird.AddNewPlayer(pid_);
        HandleCrossedGrid(old_pos, new_pos, wm);
    }
    
    pb::BroadCast pb_bc_packet;
    pb_bc_packet.set_pid(GetPid());
    pb_bc_packet.set_tp(BC_MOVE_FIELD);
    SetPosition(pb_bc_packet.mutable_p(), GetPosition());
    const zinx::ZinxPacket_LTD encoded_packet = util::packToLTDWithProtobuf(BROADCAST_PACK_ID, &pb_bc_packet);

    // Send current position to all the surrounding players
    const std::vector<Player*> surrounding_players = wm.GetSurroundingPlayers(GetPid());
    for (Player* p : surrounding_players) {
        p->SendPacket(encoded_packet);
    }
}

void mmo::Player::HandleCrossedGrid(const Position& old_pos, const Position& new_pos, const mmo::WorldManager& wm) {
    const std::vector<const Grid*> old_girds = wm.GetAoiManager().GetSudokuByPosition(old_pos);
    const std::vector<const Grid*> new_girds = wm.GetAoiManager().GetSudokuByPosition(new_pos);
    
    std::unordered_map<const Grid*, int> map;
    map.reserve(old_girds.size() + new_girds.size());
    for (const Grid* g : old_girds) {
        map[g]++;
    }

    for (const Grid* g : new_girds) {
        map[g]--;
    }

    // 找出在旧九宫格中出现但在新九宫格中没有出现的格子
    // 找出在新九宫格中出现但在旧九宫格中没有出现的格子
    std::vector<const Grid*> disappeared_grids;
    std::vector<const Grid*> nascent_grids;
    for (const auto& m_pair : map) {
        if (m_pair.second == 1) {
            disappeared_grids.push_back(m_pair.first);
        } else if (m_pair.second == -1) {
            nascent_grids.push_back(m_pair.first);
        }
    }

    std::vector<Player*> tmp_player_set;
    // 令当前玩家从超出可见范围的玩家的视野里消失
    {
        tmp_player_set = wm.GetAllPlayers(disappeared_grids);

        pb::SyncPid pb_sync_pack_to_other;
        pb_sync_pack_to_other.set_pid(GetPid());
        zinx::ZinxPacket_LTD encoded_packet = util::packToLTDWithProtobuf(SYNC_LEAVE_PACK_ID, &pb_sync_pack_to_other);

        for (Player* p : tmp_player_set) {
            p->SendPacket(encoded_packet);

            pb::SyncPid pb_sync_pack_to_self;
            pb_sync_pack_to_self.set_pid(p->GetPid());
            SendPacket(util::packToLTDWithProtobuf(SYNC_LEAVE_PACK_ID, &pb_sync_pack_to_self));
        }
    }
    
    tmp_player_set.clear();

    // 令当前玩家出现在新加入进可见范围的玩家的视野里
    {
        tmp_player_set = wm.GetAllPlayers(nascent_grids);
        pb::BroadCast pb_bc_pack_to_other;
        pb_bc_pack_to_other.set_pid(GetPid());
        pb_bc_pack_to_other.set_tp(BC_POS_FIELD);
        SetPosition(pb_bc_pack_to_other.mutable_p(), GetPosition());    // GetPosition() == new_pos
        zinx::ZinxPacket_LTD encoded_packet = util::packToLTDWithProtobuf(BROADCAST_PACK_ID, &pb_bc_pack_to_other);

        for (Player* p : tmp_player_set) {
            p->SendPacket(encoded_packet);

            pb::BroadCast pb_bc_pack_to_self;
            pb_bc_pack_to_self.set_pid(p->GetPid());
            pb_bc_pack_to_self.set_tp(BC_POS_FIELD);
            SetPosition(pb_bc_pack_to_self.mutable_p(), p->GetPosition());
            SendPacket(util::packToLTDWithProtobuf(BROADCAST_PACK_ID, &pb_bc_pack_to_self));
        }
    }
}
