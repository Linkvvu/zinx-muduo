#include <muduo/base/Logging.h>
#include <mmo_game/handler/ChatHandler.h>
#include <mmo_game/core/WorldManager.h>
#include <mmo_game/api/msg.pb.h>
#include <mmo_game/util/util.h>

using namespace mmo;

void ChatHandler::Handle(zinx::base::RequestContext& ctx) {
    int32_t pid = util::getPidFromZConnection(ctx.GetConnection());

    pb::Talk received_packet;
    bool ok = received_packet.ParseFromString(ctx.GetPacket()->GetPayload());    
    if (!ok) {
        LOG_WARN << "Failed to parse Talk-Packet in mmo::ChatHandler::Handle, detail: pid=" << pid;        
        /// FIXME: disconnect instead of only @c return
        return;
    }

    /// TODO: Get mmo::WorldManager from zinx::base::RequestContext instead of to use global variable 
    const PlayerPtr& p = mmo::GlobalWorldManager->GetPlayerByPid(pid, true);
    p->WorldChat(*received_packet.mutable_content(), *mmo::GlobalWorldManager);
}
