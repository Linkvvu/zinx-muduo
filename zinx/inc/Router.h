#if !defined(ZINX_INTERFACE_ROUTER_H)
#define ZINX_INTERFACE_ROUTER_H

#include <zinx/inc/Handler.h>
#include <zinx/inc/RequestContext.h>

namespace zinx {

/// Router's job is to route incoming requests to the handler associated with it.
class Router {
public:
    virtual void RouteAndHandle(RequestContext& req) const = 0;
    virtual ~Router() noexcept = default;
};

} // namespace zinx 

#endif // ZINX_INTERFACE_ROUTER_H
