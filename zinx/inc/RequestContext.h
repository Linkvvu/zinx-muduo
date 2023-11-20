#if !defined(ZINX_INTERFACE_REQUEST_CONTEXT_H)
#define ZINX_INTERFACE_REQUEST_CONTEXT_H

#include <muduo/TcpConnection.h>
#include <zinx/inc/Packet.h>

namespace zinx {

/// RequestContext is a abstract base class
/// Dosen`t hold the lifecycle of any object
/// Users can implement a derived class to include more context
class RequestContext : Copyable {
public:
    RequestContext(const muduo::TcpConnectionPtr& conn, Packet* p)
        : conn_(conn)
        , packet_(p)
        { }

    const muduo::TcpConnectionPtr& GetConnection() const
    { return conn_; }

    const Packet* GetPacket() const
    { return packet_; }

    Packet* GetPacket()
    { return packet_; }

    virtual ~RequestContext() noexcept = default;

private:
    const muduo::TcpConnectionPtr& conn_;   // refer to muduo::TcpConnection obj
    Packet* packet_;    // aggregation
};

} // namespace zinx 

#endif // ZINX_INTERFACE_REQUEST_CONTEXT_H
