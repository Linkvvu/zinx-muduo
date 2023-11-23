#include <muduo/base/AsyncLogging.h>
#include <muduo/base/Logging.h>
#include <zinx/inc/ZServer.h>
#include <zinx/inc/ZPacket.h>
#include <iostream>
#include <memory>

/* set async logging */
void asyncOutput(muduo::AsyncLogger* async_logger, const char* msg, size_t len) {
    async_logger->Append(msg, len);
}

class TestHandler : public zinx::Handler {
public:
    virtual void PreHandle(zinx::RequestContext& req) override {
        zinx::Handler::PreHandle(req);
    }
    
    virtual void Handle(zinx::RequestContext& req) override {
        std::cout << "Message of the Remote connection["
            << req.GetConnection()->GetName() << "]: <ID="
            << static_cast<zinx::ZinxPacket*>(req.GetPacket())->GetId()
            << "> " << req.GetPacket()->GetAllData();

        
        req.GetConnection()->Send(req.GetPacket()->GetAllData());
    }

    virtual void PostHandle(zinx::RequestContext& req) override {
        zinx::Handler::PostHandle(req);
    }
};

int main() {
    /* start async logging */
    muduo::AsyncLogger async_logger("zinx-muduo", 1024*300);
    async_logger.Start();
    muduo::Logger::SetOutputHandler(
        [async_logger = &async_logger](const char* data, size_t len) {
            async_logger->Append(data, len);
        }, false);

    /* start zinx-muduo server*/
    auto server = zinx::NewZinxServer();
    std::unique_ptr<TestHandler> t_r = std::make_unique<TestHandler>();
    server->AddHandler(1, std::make_unique<TestHandler>());
    server->ListenAndServe();
}