#include <muduo/EventLoop.h>
// #include <zinx/inc/ZPacket.h>
#include <zinx/inc/ZPacket_LTD.h>
#include <zinx/inc/ZServer.h>
#include <zinx/inc/ZRouter.h>
#include <zinx/inc/RequestContext.h>
// #include <zinx/inc/ZPacketProcessor.h>
#include <zinx/inc/ZPacketProcessor_LTD.h>
#include <iostream>

using namespace zinx;

ZinxServer::ZinxServer(const muduo::InetAddr& addr, const std::string& name) 
    : tcpServer_(&loop_, addr, name)
    , router_(std::make_unique<ZinxRouter>())
    , pp_(std::make_unique<ZinxPacketProcessor_LTD>())
{
    assert(dynamic_cast<ZinxRouter*>(router_.get()) != nullptr);
    assert(dynamic_cast<ZinxPacketProcessor_LTD*>(pp_.get()) != nullptr);

    tcpServer_.SetIoThreadNum(static_cast<int>(GlobalConfig::io_thread_num));

    PrintConfig();

    tcpServer_.SetOnMessageCallback(
        [this] (const muduo::TcpConnectionPtr& conn, muduo::Buffer* buf, muduo::ReceiveTimePoint_t tp)
        { 
            this->HandleOnMessage(conn, buf, tp);
        }
    );

    tcpServer_.SetConnectionCallback(
        [this](const muduo::TcpConnectionPtr& conn) {
            this->HandleNewConnection(conn);
        }
    );
}

bool ZinxServer::AddHandler(uint32_t id, std::unique_ptr<Handler> && handler) {
    /// FIXME: use dynamic_cast<T>(...) for the safe down-cast
    return static_cast<ZinxRouter*>(router_.get())->AddHandler(id, std::move(handler));
}

void ZinxServer::HandleOnMessage(const muduo::TcpConnectionPtr& conn, muduo::Buffer* buf, muduo::ReceiveTimePoint_t) {
    /// TODO:
    ///     1. resolve message
    ///     2. wrap RequestContext 
    ///     3. start routing  
    zinx::ZinxPacket_LTD current_packet;
    bool unpack_success = pp_->Unpcak(buf, &current_packet);
    if (!unpack_success) {
        // wait whole message
        return;
    }

    RequestContext request_context(conn, std::make_shared<ZinxPacket_LTD>(current_packet));
    
    /* route the request and handle */
    router_->RouteAndHandle(request_context);
}


void ZinxServer::PrintConfig() {
    std::cout << GlobalConfig::logo
        << "===== Global Config =====\n"
        << "[Zinx-Muduo] Version: " << GlobalConfig::version
        << "\n" << "ServerName: " << GlobalConfig::server_name
        << "\n" << "Host: " << GlobalConfig::host
        << "\n" << "Port: " << GlobalConfig::port
        << "\n" << "IO-threads: " << GlobalConfig::io_thread_num
        << "\n" << "Worker-threads: " << GlobalConfig::worker_thread_num
        << "\n" << "Maximum Task-Queue Size:" << GlobalConfig::max_task_queue_size
        << "\n";
}

void ZinxServer::HandleNewConnection(const muduo::TcpConnectionPtr& conn) {
    if (conn->IsConnected()) {
        if (onStartCb_)
            onStartCb_(conn);
    } else {
        if (onCloseCb_)
            onCloseCb_(conn);
    }
}
