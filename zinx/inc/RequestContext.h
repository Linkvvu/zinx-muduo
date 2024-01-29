#if !defined(ZINX_INTERFACE_REQUEST_CONTEXT_H)
#define ZINX_INTERFACE_REQUEST_CONTEXT_H

#include <zinx/inc/Packet.h>
#include <zinx/inc/ZConnection.h>

namespace zinx {
namespace base {
    
/// RequestContext is a abstract base class
/// The class should hold the lifecycle of all context to prevent accessing destroyed data
/// Users can implement a derived class to include more context
class RequestContext : Copyable {
public:
    RequestContext(const muduo::TcpConnectionPtr& conn, zinx::PacketPtr&& p)
        : conn_(conn)
        , packet_(std::move(p))
        { }

    const ZinxConnectionPtr& GetConnection() const
    { return conn_; }

    const Packet* GetPacket() const
    { return packet_.get(); }

    Packet* GetPacket()
    { return packet_.get(); }

    virtual ~RequestContext() noexcept = default;

private:
    ZinxConnectionPtr conn_;
    PacketPtr packet_;
};

} // namespace base 
} // namespace zinx 

#endif // ZINX_INTERFACE_REQUEST_CONTEXT_H
