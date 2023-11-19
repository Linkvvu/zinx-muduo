#if !defined(ZINX_INTERFACE_ROUTER_H)
#define ZINX_INTERFACE_ROUTER_H

#include <muduo/base/Logging.h>
#include <zinx/inc/RequestContext.h>

namespace zinx {

/// Router's job is to route incoming requests to the handler associated with it.
/// Router is a abstract base class,
/// Users should define a implementation class with service logic and inherit it
/// Default behavior of Router is do nothing  
class Router {
public:
    virtual void PreHandle(RequestContext& req) = 0;
    
    virtual void Handle(RequestContext& req) = 0;

    virtual void PostHandle(RequestContext& req) = 0;
};
/* default implementations are do nothing */
inline void Router::PreHandle(RequestContext& req) { }
inline void Router::Handle(RequestContext& req) { }
inline void Router::PostHandle(RequestContext& req) { }

} // namespace zinx 

#endif // ZINX_INTERFACE_ROUTER_H
