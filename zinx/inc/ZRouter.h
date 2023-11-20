#if !defined(ZINX_ROUTER_H)
#define ZINX_ROUTER_H

#include <muduo/base/ThreadPool.h>
#include <zinx/inc/Router.h>
#include <unordered_map>
#include <cassert>

namespace zinx {

class ZinxRouter : public Router {
public:
    ZinxRouter()
    {
        InitWorkerConfig();
    }

    bool AddHandler(uint32_t id, std::unique_ptr<Handler>&& handler);

    virtual void RouteAndHandle(RequestContext& req) const override;

    virtual ~ZinxRouter() = default;

private:

    void InitWorkerConfig();

private:
    std::unordered_map<uint32_t, std::unique_ptr<Handler>> mapper_ {};
    mutable muduo::ThreadPool workerThreads_ {};
};

} // namespace zinx 

#endif // ZINX_ROUTER_H
