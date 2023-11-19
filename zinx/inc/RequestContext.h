#if !defined(ZINX_INTERFACE_REQUEST_CONTEXT_H)
#define ZINX_INTERFACE_REQUEST_CONTEXT_H

#include <muduo/TcpConnection.h>
#include <zinx/inc/Copyable.h>
#include <zinx/inc/Packet.h>

namespace zinx {

class RequestContext {
public:
    RequestContext(const muduo::TcpConnectionPtr& conn, Packet* p)
        : conn_(conn)
        , packet_(p)
        { }

    const muduo::TcpConnectionPtr& GetConnection() const
    { return conn_; }

    Packet* GetPacket()
    { return packet_; }

    virtual ~RequestContext() noexcept = default;

private:
    const muduo::TcpConnectionPtr& conn_;   // refer to muduo::TcpConnection obj
    Packet* packet_;    // aggregation
};

} // namespace zinx 

#endif // ZINX_INTERFACE_REQUEST_CONTEXT_H
