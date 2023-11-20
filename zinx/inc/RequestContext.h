#if !defined(ZINX_INTERFACE_REQUEST_CONTEXT_H)
#define ZINX_INTERFACE_REQUEST_CONTEXT_H

#include <muduo/TcpConnection.h>
#include <zinx/inc/Packet.h>

namespace zinx {

/// RequestContext is a abstract base class
/// The class Should hold the lifecycle of all context to prevent accessing destroyed data
/// Users can implement a derived class to include more context
class RequestContext : Copyable {
public:
    RequestContext(const muduo::TcpConnectionPtr& conn, const zinx::PacketPtr& p)
        : conn_(conn)
        , packet_(p)
        { }

    const muduo::TcpConnectionPtr& GetConnection() const
    { return conn_; }

    const Packet* GetPacket() const
    { return packet_.get(); }

    Packet* GetPacket()
    { return packet_.get(); }

    virtual ~RequestContext() noexcept = default;

private:
    muduo::TcpConnectionPtr conn_;
    PacketPtr packet_;
};

} // namespace zinx 

#endif // ZINX_INTERFACE_REQUEST_CONTEXT_H
