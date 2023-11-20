#if !defined(ZINX_SERVER_H)
#define ZINX_SERVER_H

#include <muduo/TcpServer.h>
#include <muduo/EventLoop.h>
#include <zinx/inc/Router.h>
#include <zinx/inc/PacketProcessor.h>

namespace zinx {

extern muduo::MessageCallback_t g_messageCb;

/// non-copyable & non-moveable
class ZinxServer {
public:
    /// constructor
    ZinxServer(const muduo::InetAddr& addr, const std::string& name);

    /**
     * @code {.cpp}
     * using muduo::TcpServer::SetIoThreadNum();
     * @endcode
     * Set the number of I/O threads by config file
     */

    /// Run the server and block the process
    void ListenAndServe() {
        /* run server */
        tcpServer_.ListenAndServe();
        /* starts looping */
        loop_.Loop();
    }
    
    bool AddHandler(uint32_t id, std::unique_ptr<Handler>&& handler);

private:
    void HandleNewConnection(const muduo::TcpConnectionPtr& conn); 
    void HandleOnMessage(const muduo::TcpConnectionPtr& conn, muduo::Buffer* buf, muduo::ReceiveTimePoint_t);

private:
    muduo::EventLoop loop_ {};      // Must be declared on this line, Ensuring a longer lifecycle than other data-members
    muduo::TcpServer tcpServer_;   
    std::unique_ptr<Router> router_;
    std::unique_ptr<PacketProcessor> pp_;
};

} // namespace zinx 

#endif // ZINX_SERVER_H
