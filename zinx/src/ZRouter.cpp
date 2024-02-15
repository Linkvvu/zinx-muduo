#include <muduo/base/Logging.h>
#include <zinx/inc/ZConfig.h>
#include <zinx/inc/ZRouter.h>
#include <zinx/inc/TagPacket.h>

using namespace zinx;

void ZinxRouter::RouteAndHandle(base::RequestContext& req) const {
    assert(dynamic_cast<const base::TagPacket*>(req.GetPacket()) != nullptr);

    /// FIXME: use dynamic_cast for the safe down-cast
    const base::TagPacket* z_packet = static_cast<const base::TagPacket*>(req.GetPacket());
    auto it = mapper_.find(z_packet->GetId());
    if (it == mapper_.end()) {
        LOG_WARN << "Doesn`t find a available handler for request from [" << req.GetConnection()->GetRemoteAddr().GetIpPort()
            << "], detail: " << "zinx packet id=" << z_packet->GetId();
        return;
    }

    zinx::Handler* handler = it->second.get();

    /* handles the request*/
    handler->PreHandle(req);
    handler->Handle(req);
    handler->PostHandle(req);
}

bool ZinxRouter::AddHandler(uint32_t id, std::unique_ptr<Handler> && handler) {
    const auto it = mapper_.find(id);
    if (it == mapper_.end()) {
        mapper_[id] = std::move(handler);  // should update handler ?
        return false;
    } 
    mapper_[id] = std::move(handler);
    return true;
}
