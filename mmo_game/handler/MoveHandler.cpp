#include <mmo_game/handler/MoveHandler.h>
#include <mmo_game/core/WorldManager.h>
#include <mmo_game/api/msg.pb.h>

using namespace mmo;

void MoveHandler::Handle(zinx::RequestContext& req) {
    int32_t pid;
    try {
        pid = std::any_cast<int32_t>(req.GetConnection()->GetContext());
    } catch(const std::bad_any_cast& e) {
        std::cerr << e.what() << '\n';
        return;
    }
    mmo::Player* cur_player = mmo::GlobalWorldManager->GetPlayerByPid(pid);

    mmo::pb::Position pos_packet;
    bool parse_ok = pos_packet.ParseFromString(req.GetPacket()->GetPayload());
    if (!parse_ok) {
        std::cerr << "MoveHandler::Handle Failed to parse protobuf in player " << pid << std::endl;
        return;
    }

    mmo::Position mmo_pos;
    mmo_pos.X = pos_packet.x();
    mmo_pos.Y = pos_packet.y();
    mmo_pos.Z = pos_packet.z();
    mmo_pos.V = pos_packet.v();

    cur_player->UpdatePosition(mmo_pos, *mmo::GlobalWorldManager);
}