#if !defined(ZINX_ROUTER_H)
#define ZINX_ROUTER_H

#include <zinx/inc/Router.h>
#include <unordered_map>
#include <cassert>

namespace zinx {

class ZinxRouter : public Router {
public:
    bool AddHandler(uint32_t id, std::unique_ptr<Handler>&& handler) 
    {
        const auto it = mapper_.find(id);
        if (it == mapper_.end()) {
            mapper_[id] = std::move(handler);  // should update handler ?
            return false;
        } 
        mapper_[id] = std::move(handler);
        return true;
    }

    virtual void RouteAndHandle(RequestContext& req) const override {
        assert(dynamic_cast<const ZinxPacket*>(req.GetPacket()) != nullptr);

        const ZinxPacket* z_packet = static_cast<const ZinxPacket*>(req.GetPacket());
        auto it = mapper_.find(z_packet->GetId());
        if (it == mapper_.end()) {
            LOG_WARN << "Doesn`t find a available handler for request from [" << req.GetConnection()->GetRemoteAddr().GetIpPort()
                << "], detail: " << "zinx packet id=" << z_packet->GetId();
            return;
        }

        /* handle */
        (it->second)->PreHandle(req);
        (it->second)->Handle(req);
        (it->second)->PostHandle(req);
    }

    virtual ~ZinxRouter() = default;
private:
    std::unordered_map<uint32_t, std::unique_ptr<Handler>> mapper_ {};
};

} // namespace zinx 

#endif // ZINX_ROUTER_H
