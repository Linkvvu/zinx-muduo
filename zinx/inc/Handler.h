#if !defined(ZINX_HANDLER_H)
#define ZINX_HANDLER_H

#include <zinx/inc/RequestContext.h>

namespace zinx {

/// Handler is a abstract base class,
/// Users should define a implementation class with service logic and inherit it
/// Default behavior of Handler is do nothing  
class Handler {
public:
    virtual void PreHandle(/* const */ RequestContext& req) {}
    
    virtual void Handle(/* const */ RequestContext& req) = 0;

    virtual void PostHandle(/* const */ RequestContext& req) {};

    virtual ~Handler() noexcept = default;
};

/* default implementations are do nothing */
inline void Handler::Handle(RequestContext& req) { }

} // namespace zinx 



#endif // ZINX_HANDLER_H
