#if !defined(MMO_HANDLER_WORLD_CHAT_HANDLER_H)
#define MMO_HANDLER_WORLD_CHAT_HANDLER_H
#include <zinx/inc/Handler.h>

namespace mmo {

class WorldChatHandler : public zinx::Handler {
public:
    virtual void PreHandle(zinx::RequestContext& req) override
    { zinx::Handler::PreHandle(req); }
    
    virtual void Handle(zinx::RequestContext& req) override;

    virtual void PostHandle(zinx::RequestContext& req) override
    { zinx::Handler::PostHandle(req); }
};

} // namespace mmo
#endif // MMO_HANDLER_WORLD_CHAT_HANDLER_H
