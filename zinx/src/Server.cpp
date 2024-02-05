#include <zinx/inc/Server.h>
#include <zinx/inc/ZConfig.h>
#include <zinx/inc/Decoder.h>
#include <iostream>

using namespace zinx;

namespace {

static void PrintConfig() {
    std::cout << base::GlobalConfig::logo
        << "====== Global Config ======\n"
        << "[Zinx-Muduo] Version: " << base::GlobalConfig::version
        << "\n" << "ServerName: " << base::GlobalConfig::server_name
        << "\n" << "Host: " << base::GlobalConfig::host
        << "\n" << "Port: " << base::GlobalConfig::port
        << "\n" << "IO-threads: " << base::GlobalConfig::io_thread_num
        << "\n" << "Worker-threads: " << base::GlobalConfig::worker_thread_num
        << "\n" << "Maximum Task-Queue Size:" << base::GlobalConfig::max_task_queue_size
        << "\n";
}

} // namespace 


base::AbstractServer::AbstractServer(const muduo::InetAddr& addr, const std::string& name)
    : loop_()
    , tcpServer_(muduo::TcpServer::Create(&loop_, addr, name))
{
    PrintConfig();
    tcpServer_->SetIoThreadNum(static_cast<int>(GlobalConfig::io_thread_num));

    tcpServer_->SetConnectionCallback(
        [this](const muduo::TcpConnectionPtr& conn) {
            this->HandleNewConnection(conn);
        }
    );
    
    tcpServer_->SetOnMessageCallback(
        [this] (const muduo::TcpConnectionPtr& conn, muduo::Buffer* buf, muduo::ReceiveTimePoint_t tp) { 
            this->HandleOnMessage(conn, buf, tp);
        }
    );
}

void base::AbstractServer::HandleNewConnection(const muduo::TcpConnectionPtr& conn) {
    if (conn->IsConnected()) {
        if (onStartCb_)
            onStartCb_(conn);
    } else {
        if (onCloseCb_)
            onCloseCb_(conn);
    }
}

void base::AbstractServer::HandleOnMessage(const muduo::TcpConnectionPtr& conn, muduo::Buffer* buf, const muduo::ReceiveTimePoint_t& recvTime) {
    while (true) {
        // resolves all the incoming data
        PacketPtr packet_ptr = decoder_->Decode(buf);
        if (!packet_ptr) {
            // incomplete pack, wait for the new read event 
            return;
        }
        std::shared_ptr<RequestContext> req_context = WrapRequestCtx(conn, std::move(packet_ptr));
        router_->RouteAndHandle(*req_context);
    }
}