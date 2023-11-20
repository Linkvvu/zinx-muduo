#if !defined(ZINX_HANDLER_H)
#define ZINX_HANDLER_H

#include <zinx/inc/RequestContext.h>

namespace zinx {

/// Handler is a abstract base class,
/// Users should define a implementation class with service logic and inherit it
/// Default behavior of Handler is do nothing  
class Handler {
public:
    virtual void PreHandle(RequestContext& req) = 0;
    
    virtual void Handle(RequestContext& req) = 0;

    virtual void PostHandle(RequestContext& req) = 0;

    virtual ~Handler() noexcept = default;
};

/* default implementations are do nothing */
inline void Handler::PreHandle(RequestContext& req) { }
inline void Handler::Handle(RequestContext& req) { }
inline void Handler::PostHandle(RequestContext& req) { }

} // namespace zinx 



#endif // ZINX_HANDLER_H
