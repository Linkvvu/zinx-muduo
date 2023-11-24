#if !defined(ZINX_SERVER_H)
#define ZINX_SERVER_H

#include <muduo/TcpServer.h>
#include <muduo/EventLoop.h>
#include <zinx/inc/Router.h>
#include <zinx/inc/ZConfig.h>
#include <zinx/inc/ZConnection.h>
#include <zinx/inc/PacketProcessor.h>

namespace zinx {
class ZinxServer;   // forward declaration

/// TODO: implement Singleton pattern for zinx::NewZinxServer
/// factory method
/// initialize global config and create a ZinxServer instance
std::unique_ptr<ZinxServer> NewZinxServer();

/// define a alias for callback of ZinxConnection 
using ConnectionStartCallback_t = muduo::ConnectionCallback_t;
using ConnectionCloseCallback_t = muduo::ConnectionCallback_t;

/// non-copyable & non-moveable
class ZinxServer {
    friend std::unique_ptr<ZinxServer> NewZinxServer();
    /// private constructor
    ZinxServer(const muduo::InetAddr& addr, const std::string& name);

public:
    /**
     * @code {.cpp}
     * using muduo::TcpServer::SetIoThreadNum();
     * @endcode
     * Set the number of I/O threads by config file
     */

    /// Run the server and block the process
    /// Must be call in the same thread as creating of the ZinxServer.
    void ListenAndServe() {
        /* run server */
        tcpServer_.ListenAndServe();
        /* starts looping */
        loop_.Loop();
    }

    /// stop internal event-loop and then to stop the ZinxServer
    /// @note thread-safe
    void Stop() 
    { loop_.Quit(); }
    
    bool AddHandler(uint32_t id, std::unique_ptr<Handler>&& handler);

    void SetOnConnStart(const ConnectionStartCallback_t& cb)
    { onStartCb_ = cb; }

    void SetOnConnClose(const ConnectionCloseCallback_t& cb)
    { onCloseCb_ = cb; }

private:
    void HandleNewConnection(const muduo::TcpConnectionPtr& conn); 
    void HandleOnMessage(const muduo::TcpConnectionPtr& conn, muduo::Buffer* buf, muduo::ReceiveTimePoint_t);

    void PrintConfig();

private:
    muduo::EventLoop loop_ {};      // Must be declared on this line, Ensuring a longer lifecycle than other data-members
    muduo::TcpServer tcpServer_;   
    std::unique_ptr<Router> router_;
    std::unique_ptr<PacketProcessor> pp_;

    ConnectionStartCallback_t onStartCb_ {nullptr};
    ConnectionCloseCallback_t onCloseCb_ {nullptr};
};

inline std::unique_ptr<ZinxServer> NewZinxServer() {
    /* reload config */
    GlobalConfig::InitConfig();
    std::unique_ptr<ZinxServer> server;
    server.reset(new ZinxServer(muduo::InetAddr(GlobalConfig::host, GlobalConfig::port),
                            GlobalConfig::server_name));
    return server;
}

} // namespace zinx 

#endif // ZINX_SERVER_H
