#include <muduo/base/Logging.h>
#include <mmo_game/handler/MoveHandler.h>
#include <mmo_game/core/WorldManager.h>
#include <mmo_game/api/msg.pb.h>
#include <mmo_game/util/util.h>

using namespace mmo;

void MoveHandler::Handle(zinx::RequestContext& ctx) {
    int32_t pid = util::getPidFromZConnection(ctx.GetConnection());    
    pb::Position received_packet;
    bool ok = received_packet.ParseFromString(ctx.GetPacket()->GetPayload());
    if (!ok) {
        LOG_WARN << "Failed to parse Position-Packet in mmo::MoveHandler::Handle, detail: pid=" << pid;        
        /// FIXME: disconnect instead of only @c return
        return;
    }

    /// TODO: Get mmo::WorldManager from zinx::RequestContext instead of to use global variable 
    const PlayerPtr& p = mmo::GlobalWorldManager->GetPlayerByPid(pid);
    p->UpdatePos(mmo::Position{
            static_cast<int>(received_packet.x()), static_cast<int>(received_packet.y()),
            static_cast<int>(received_packet.z()), static_cast<int>(received_packet.v())
        }, *mmo::GlobalWorldManager
    );
}
