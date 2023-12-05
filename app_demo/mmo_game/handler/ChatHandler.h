#if !defined(MMO_HANDLER_CHAT_H)
#define MMO_HANDLER_CHAT_H

#include <zinx/inc/Handler.h>

namespace mmo {

class ChatHandler : public zinx::Handler {
public:
    virtual void Handle(zinx::RequestContext& ctx) override;
};

} // namespace mmo 

#endif // MMO_HANDLER_CHAT_H
