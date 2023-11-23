#if !defined(ZINX_SERVER_H)
#define ZINX_SERVER_H

#include <muduo/TcpServer.h>
#include <muduo/EventLoop.h>
#include <zinx/inc/Router.h>
#include <zinx/inc/ZConfig.h>
#include <zinx/inc/PacketProcessor.h>

namespace zinx {
class ZinxServer;   // forward declaration

extern std::unique_ptr<ZinxServer> NewZinxServer();

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

    void SetOnConnStart(const muduo::ConnectionCallback_t& cb)
    { onStartCb_ = cb; }

    void SetOnConnClose(const muduo::ConnectionCallback_t& cb)
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

    muduo::ConnectionCallback_t onStartCb_ {nullptr};
    muduo::ConnectionCallback_t onCloseCb_ {nullptr};
};


} // namespace zinx 

#endif // ZINX_SERVER_H
