#include <muduo/TcpClient.h>
#include <muduo/TcpConnection.h>
#include <zinx/inc/ZPacket.h>

void Ping(const muduo::TcpConnectionPtr& conn) {
    static int32_t num = 0;
    zinx::ZinxPacket pack(1, std::string("ping ") += std::to_string(num++));
    conn->Send(pack.GetData());
}

void OnClientStart(const muduo::TcpConnectionPtr& conn) {
    if (conn->IsConnected()) {
        LOG_INFO << "::OnClientStart is invoking...";
        conn->GetEventLoop()->RunEvery(std::chrono::seconds(1), [conn]() {
            ::Ping(conn);
        });
    }
};

int main() {
    // create a EventLoop instance
    muduo::EventLoop loop;
    // create the client instance
    muduo::InetAddr addr("127.0.0.1", 8999);
    muduo::TcpClientPtr client = muduo::CreateTcpClient(&loop, std::move(addr), "echo-client");
    // registers hook function
    client->SetConnectionCallback(&OnClientStart);
    // connect
    client->Connect();
    // start looping
    loop.Loop();
    // wait signal...
}