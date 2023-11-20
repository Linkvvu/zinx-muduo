#include <muduo/EventLoop.h>
#include <muduo/TcpConnection.h>
#include <zinx/inc/ZPacket.h>
#include <zinx/inc/ZServer.h>
#include <zinx/inc/ZinxRouter.h>
#include <zinx/inc/RequestContext.h>
#include <zinx/inc/ZPacketProcessor.h>
#include <iostream>

using namespace zinx;

ZinxServer::ZinxServer(const muduo::InetAddr& addr, const std::string& name) 
    : tcpServer_(&loop_, addr, name)
    , router_(std::make_unique<ZinxRouter>())
    , pp_(std::make_unique<ZinxPacketProcessor>())
{
    assert(dynamic_cast<ZinxRouter*>(router_.get()) != nullptr);

    tcpServer_.SetOnMessageCallback(
        [this] (const muduo::TcpConnectionPtr& conn, muduo::Buffer* buf, muduo::ReceiveTimePoint_t tp)
        { 
            this->HandleOnMessage(conn, buf, tp);
        }
    );
    tcpServer_.SetConnectionCallback(std::bind(&ZinxServer::HandleNewConnection, this, std::placeholders::_1));
}

bool ZinxServer::AddHandler(uint32_t id, std::unique_ptr<Handler> && handler) {
    /// FIXME: use dynamic_cast<T>(...) for A safe down-cast
    return static_cast<ZinxRouter*>(router_.get())->AddHandler(id, std::move(handler));
}

void ZinxServer::HandleOnMessage(const muduo::TcpConnectionPtr& conn, muduo::Buffer* buf, muduo::ReceiveTimePoint_t) {
    /// TODO:
    ///     1. resolve message
    ///     2. wrap RequestContext 
    ///     3. start routing  
    zinx::ZinxPacket current_packet;
    bool unpack_success = pp_->Unpcak(buf, &current_packet);
    if (!unpack_success) {
        // wait whole message
        return;
    }

    RequestContext request_context(conn, &current_packet);
    
    /* route the request and handle */
    router_->RouteAndHandle(request_context);
}

void ZinxServer::HandleNewConnection(const muduo::TcpConnectionPtr& conn) {
    /// TODO: 

}
