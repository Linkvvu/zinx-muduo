#if !defined(ZINX_ROUTER_H)
#define ZINX_ROUTER_H

#include <muduo/base/ThreadPool.h>
#include <zinx/inc/Router.h>
#include <unordered_map>
#include <cassert>

namespace zinx {

class ZinxRouter : public base::Router {
public:
    ZinxRouter() = default;

    bool AddHandler(uint32_t id, std::unique_ptr<Handler>&& handler);

    virtual void RouteAndHandle(base::RequestContext& req) const override;

    virtual ~ZinxRouter() = default;

private:
    std::unordered_map<uint32_t, std::unique_ptr<Handler>> mapper_;
};

} // namespace zinx 

#endif // ZINX_ROUTER_H
