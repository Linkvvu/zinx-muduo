#include <zinx/inc/ZServer.h>
#include <zinx/inc/ZPacket.h>
#include <iostream>
#include <memory>

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
    zinx::ZinxServer server(muduo::InetAddr(8888), "zinx-v0.1");
    std::unique_ptr<TestHandler> t_r = std::make_unique<TestHandler>();
    server.AddHandler(1, std::make_unique<TestHandler>());
    server.ListenAndServe();
}