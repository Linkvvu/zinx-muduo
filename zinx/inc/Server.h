#if !defined(ZINX_INTERFACE_SERVER_H)
#define ZINX_INTERFACE_SERVER_H

#include <muduo/EventLoop.h>
#include <muduo/TcpServer.h>
#include <muduo/TcpConnection.h>
#include <muduo/base/ThreadPool.h>
#include <zinx/inc/Router.h>
#include <zinx/inc/Decoder.h>
#include <zinx/inc/NonCopyable.h>
#include <zinx/inc/RequestContext.h>
#include <memory>

namespace zinx {

/// define a alias for callback of ZinxConnection 
using ConnectionStartCallback_t = muduo::ConnectionCallback_t;
using ConnectionCloseCallback_t = muduo::ConnectionCallback_t;

namespace base {

class AbstractServer : protected NonCopyable {
protected:
    AbstractServer() = delete;
    AbstractServer(const muduo::InetAddr& addr, const std::string& name);
public:

    virtual ~AbstractServer() noexcept = default;

    /**
     * @code {.cpp}
     * using muduo::TcpServer::SetIoThreadNum();
     * @endcode
     * Set the number of I/O threads by config file
     */

    /// Run the server and block the process
    /// Must be call in the same thread as creating of the ZinxServer.
    void ListenAndServe() {
        /* run the server */
        tcpServer_->ListenAndServe();
        LOG_INFO << "Server is listening at " << tcpServer_->GetIpPort();

        /* starts looping */
        loop_.Loop();
    }

    /// stop internal event-loop and then to stop the ZinxServer
    /// @note thread-safe
    void Stop() 
    {
        loop_.Quit();
        LOG_INFO << "Event-Loop is about to stop!";
    }

    void SetOnConnStart(const ConnectionStartCallback_t& cb)
    { onStartCb_ = cb; }

    void SetOnConnClose(const ConnectionCloseCallback_t& cb)
    { onCloseCb_ = cb; }

    void SetRouter(std::unique_ptr<zinx::base::Router>&& router)
    { router_ = std::move(router); }

    void SetDecoder(std::unique_ptr<zinx::base::Decoder>&& decoder)
    { decoder_ = std::move(decoder); }

protected:

    void HandleNewConnection(const muduo::TcpConnectionPtr& conn); 
    void HandleOnMessage(const muduo::TcpConnectionPtr& conn, muduo::Buffer* buf, const muduo::ReceiveTimePoint_t& recvTime);

    virtual std::shared_ptr<RequestContext> WrapRequestCtx(const muduo::TcpConnectionPtr& conn, PacketPtr packet)
    { return std::make_shared<RequestContext>(conn, std::move(packet)); }


protected:
    muduo::EventLoop loop_;   // Must be declared on this line, ensuring a longer lifecycle than other data-members
    std::unique_ptr<muduo::TcpServer> tcpServer_;
    std::unique_ptr<zinx::base::Router> router_   {nullptr};
    std::unique_ptr<zinx::base::Decoder> decoder_ {nullptr};

    ConnectionStartCallback_t onStartCb_ {nullptr};
    ConnectionCloseCallback_t onCloseCb_ {nullptr};
};

} // namespace base  

}

#endif // ZINX_INTERFACE_SERVER_H
