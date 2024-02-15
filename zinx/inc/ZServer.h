#if !defined(ZINX_SERVER_H)
#define ZINX_SERVER_H

#include <zinx/inc/Server.h>
#include <zinx/inc/ZRouter.h>
#include <zinx/inc/ZConfig.h>

namespace zinx {
class ZinxServer;   // forward declaration

/// TODO: implement Singleton pattern for zinx::NewZinxServer
/// factory method, initialize global config and create a ZinxServer instance
std::unique_ptr<ZinxServer> NewZinxServer();

/// non-copyable & non-moveable
class ZinxServer : public base::AbstractServer {
    friend std::unique_ptr<ZinxServer> NewZinxServer();
    ZinxServer(const muduo::InetAddr& addr, const std::string& name);
    
public:
    bool AddHandler(uint32_t id, std::unique_ptr<Handler>&& handler) 
    {
        LOG_INFO << "Add API id=" << id;
        return static_cast<ZinxRouter*>(router_.get())->AddHandler(id, std::move(handler));
    }

private:
    using AbstractServer::WrapRequestCtx;

private:
    /// worker threads
    std::unique_ptr<muduo::ThreadPool> workerPool_;
};

inline std::unique_ptr<ZinxServer> NewZinxServer() {
    // loading config
    zinx::base::GlobalConfig::InitConfig();
    std::unique_ptr<ZinxServer> server(new ZinxServer(muduo::InetAddr(base::GlobalConfig::host, base::GlobalConfig::port),
                                        base::GlobalConfig::server_name));
    return server;
}

} // namespace zinx 

#endif // ZINX_SERVER_H
