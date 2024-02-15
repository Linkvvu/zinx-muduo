# Require
### Upstream project
https://github.com/VV-hao/muduo-study
### Compile
``` bash
# export MuduoNet_DIR=${real_path}
bash build.sh
```

# Quick Start
### Server
```cpp
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
```

**Run Server**
```bash
$ ./build/bin/echo-server

 _____    _                      __  ___            __
/__  /   (_)____   _  __        /  |/  /__  __ ____/ /__  __ ____
  / /   / // __ \ | |/_/______ / /|_/ // / / // __  // / / // __ \
 / /__ / // / / /_>  < /_____// /  / // /_/ // /_/ // /_/ // /_/ /
/____//_//_/ /_//_/|_|       /_/  /_/ \__,_/ \__,_/ \__,_/ \____/

====== Global Config ======
[Zinx-Muduo] Version: V0.1
ServerName: zinx-muduo
Host: 0.0.0.0
Port: 8999
IO-threads: 3
Worker-threads: 0
Maximum Task-Queue Size:30
[INFO ] 2024-02-14 08:24:17.353 Tid=139775888365952 Add API id=1 - ZServer.h:23
[INFO ] 2024-02-14 08:24:17.354 Tid=139775888365952 Server is listening at 0.0.0.0:8999 - Server.h:42
```

### Client
```cpp
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
```

**Terminal of zinx-muduo print:** 
```bash
[INFO ] 2024-02-14 08:30:04.873 Tid=140695595050752 the connection zinx-muduo127.0.0.1:42722@0 is connected from 127.0.0.1:42722 - Server.cpp:19
[INFO ] 2024-02-14 08:30:05.874 Tid=140695595050752 recv from client : msgId=1, data=ping 0 - Server.cpp:10
[INFO ] 2024-02-14 08:30:06.874 Tid=140695595050752 recv from client : msgId=1, data=ping 1 - Server.cpp:10
[INFO ] 2024-02-14 08:30:07.874 Tid=140695595050752 recv from client : msgId=1, data=ping 2 - Server.cpp:10
[INFO ] 2024-02-14 08:30:08.874 Tid=140695595050752 recv from client : msgId=1, data=ping 3 - Server.cpp:10
[INFO ] 2024-02-14 08:30:09.874 Tid=140695595050752 recv from client : msgId=1, data=ping 4 - Server.cpp:10
...
```

### zinx-muduo配置文件
```json
{
    "server_name": "zinx-muduo",
    "version": "V0.1",
    "host": "127.0.0.1",
    "port": 0,
    "io_thread_num": 3,
    "worker_thread_num": 0,
    "max_task_queue_size": 30
}
```

`io_thread_num`：IO线程数量（Reactor模式选择）=0：single-Reactor；>0：multiple-Reactor

`worker_thread_num`：工作线程数量

`max_task_queue_size`：任务队列（为工作线程提供任务）大小

# MMO Server
### Docker一键启动Server
![](https://gh-images.oss-cn-beijing.aliyuncs.com/zinx-muduo/run%20by%20Docker.gif)
### 演示
![](https://gh-images.oss-cn-beijing.aliyuncs.com/zinx-muduo/MMO-server.gif)
