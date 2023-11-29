#include <mmo_game/handler/WorldChatHandler.h> 
#include <mmo_game/core/WorldManager.h>
#include <mmo_game/api/msg.pb.h>
#include <iostream>
using namespace mmo;

void WorldChatHandler::Handle(zinx::RequestContext& req) {
    int32_t pid;
    try {
        pid = std::any_cast<int32_t>(req.GetConnection()->GetContext());
    } catch(const std::bad_any_cast& e) {
        std::cerr << e.what() << '\n';
    }

    mmo::Player* cur_player = mmo::GlobalWorldManager->GetPlayerByPid(pid);
    mmo::pb::Talk talk_packet;
    bool parse_ok = talk_packet.ParseFromString(req.GetPacket()->GetPayload());
    if (!parse_ok) {
        std::cerr << "Failed to parse protobuf in player " << pid << std::endl;
        return;
    }

    cur_player->WorldChat(talk_packet.content(), *mmo::GlobalWorldManager);
}
