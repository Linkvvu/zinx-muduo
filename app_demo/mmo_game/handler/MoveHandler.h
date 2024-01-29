#if !defined(MMO_HANDLER_MOVE_H)
#define MMO_HANDLER_MOVE_H

#include <zinx/inc/Handler.h>

namespace mmo {
class MoveHandler : public zinx::Handler {
public:
    virtual void Handle(zinx::base::RequestContext& ctx) override;
};

} // namespace mmo 

#endif // MMO_HANDLER_MOVE_H
