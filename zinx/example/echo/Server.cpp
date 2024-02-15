#include <zinx/inc/ZServer.h>
#include <zinx/inc/ZDecoder.h>
#include <zinx/inc/TagPacket.h>

/// @brief A echo-handler
struct EchoHandler : public zinx::Handler {

    void Handle(zinx::base::RequestContext& context) override {
        std::string&& payload = context.GetPacket()->GetPayload();
        LOG_INFO << "recv from client : msgId="
                << static_cast<const zinx::base::TagPacket*>(context.GetPacket())->GetId()
                << ", data=" << payload;
        context.GetConnection()->Send(std::move(payload));
    }

};

void OnConnected(const zinx::ZinxConnectionPtr& conn) {
    LOG_INFO << "the connection " << conn->GetName() << " is connected from " << conn->GetRemoteAddr().GetIpPort();
}

void OnDisConnected(const zinx::ZinxConnectionPtr& conn) {
    LOG_INFO << "the connection " << conn->GetName() << " will disconnect";
}

int main() {
    // create the server instance
    std::unique_ptr<zinx::ZinxServer> echo_server = zinx::NewZinxServer();
    // registers the Decoder
    echo_server->SetDecoder(std::make_unique<zinx::ZinxDecoder>());
    // registers the callback on connected
    echo_server->SetOnConnStart(&OnConnected);
    // registers the callback on disconnected
    echo_server->SetOnConnClose(&OnDisConnected);
    // registers the echo-handler
    echo_server->AddHandler(1, std::make_unique<EchoHandler>());
    // serves
    echo_server->ListenAndServe();
}
