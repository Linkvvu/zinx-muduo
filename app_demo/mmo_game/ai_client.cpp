#include <muduo/TcpClient.h>
#include <muduo/TcpConnection.h>
#include <zinx/inc/ZDecoder_LTD.h>
#include <zinx/inc/ZPacket_LTD.h>
#include <mmo_game/api/msg_id.h>
#include <mmo_game/api/msg.pb.h>
#include <future>
#include <random>
#include <string>
#include <cassert>

class Player {
public:
    explicit Player(muduo::TcpClientPtr&& client)
        : client_(std::move(client))
        , promise_()
        , future_(promise_.get_future())
    {
        // set the callback
        client_->SetOnMessageCallback([this](const muduo::TcpConnectionPtr& conn, muduo::Buffer* input_buf, muduo::ReceiveTimePoint_t tp) {
            HandleMessage(conn, input_buf, tp);
        });

        // notifies the event that the player exiting
        client_->SetConnectionCallback([this](const muduo::TcpConnectionPtr& conn) {
            if (conn->IsConnected()) {
                conn->SetTcpNoDelay(true);
            } else {
                conn->GetEventLoop()->cancelTimer(this->timerId_);  // curent thread is loop-thread, so the timer will be canceled syncly
                promise_.set_value();
            }
        });
    }

    // join to the game
    void Join() {
        client_->Connect();
    }
    
    void WaitPlayExit() const {
        future_.wait();
    }

private:
    /// Get a complete pack 
    std::unique_ptr<zinx::ZinxPacket_LTD> GetPack(muduo::Buffer* input_buf) const {
        zinx::PacketPtr raw_pack = decoder.Decode(input_buf);
        std::unique_ptr<zinx::ZinxPacket_LTD> pack(static_cast<zinx::ZinxPacket_LTD*>(raw_pack.release()));
        return pack;
    }

    void HandleMessage(const muduo::TcpConnectionPtr& conn, muduo::Buffer* input_buf, muduo::ReceiveTimePoint_t tp) {
        while (true) {
            // resolves all the incoming data
            auto pack = GetPack(input_buf);
            if (!pack) {
                // incomplete pack, wait for the new read event 
                return;
            }
            std::string payload = pack->GetPayload();
            if (pack->GetId() == SYNC_PID_PACK_ID) {
                SyncPid(payload);
            } else if (pack->GetId() == BROADCAST_PACK_ID) {
                HandleBroadcast(payload);
            } else {
                // omits the events of player disappearing and moving
            }
        }
    }

    /// Gets a PID from server
    void SyncPid(const std::string& payload) {
        mmo::pb::SyncPid sync_pid_pack;
        bool ok = sync_pid_pack.ParseFromString(payload);
        if (!ok) {
            LOG_ERROR << "the client[" << client_->GetName() << id_ << "] received a broken <SyncPack>, will disconnect.";
            client_->Shutdown();
        } else {
            id_ = sync_pid_pack.pid();
        }
    }

    /// Handle the broadcast pack
    void HandleBroadcast(const std::string& payload) {
        mmo::pb::BroadCast broadcast_pack;
        bool ok = broadcast_pack.ParseFromString(payload);
        if (!ok) {
            LOG_WARN << "the client[" << client_->GetName() << id_ << "] received a broken <BroadcastPack>, ignoring...";
        } else {
            if (broadcast_pack.tp() == BC_POS_FIELD && broadcast_pack.pid() == id_) {
                // The init-position-data of current client
                pos_.X = broadcast_pack.p().x();
                pos_.Y = broadcast_pack.p().y();
                pos_.Z = broadcast_pack.p().z();
                pos_.V = broadcast_pack.p().v();
                LOG_INFO << "the client[" << client_->GetName() << id_ << "]'s initial position: {"
                        << "x=" << pos_.X << ", y=" << pos_.Y
                        << ", z=" << pos_.Z << ", v=" << pos_.V
                        << "}";
                
                // the player has been initialized over, starts to do actions
                timerId_ = client_->GetEventLoop()->RunEvery(std::chrono::seconds(1), [this]() {
                    // doesn't worry about that the callback accessing the destroyed Player instance
                    // the timer will be canceled by TcpConnection::ConnectionCallback,
                    this->AiRobotAction();
                });
                
                // closes the client after 15s 
                client_->GetEventLoop()->RunAfter(std::chrono::seconds(15), [this]() {
                    this->client_->Shutdown();
                });

            } else if (broadcast_pack.tp() == BC_TALK_FIELD) {
                LOG_INFO << "the client[" << client_->GetName() << id_ << "] receive a msg: " << broadcast_pack.content();
            }
        }
    }

    /// Pack and send the pack to server 
    void DoPackAndSend(int id, const google::protobuf::Message* msg) const {
        std::string payload = msg->SerializeAsString();
        if (payload.empty()) {
            LOG_WARN << "the client[" << client_->GetName() << id_ << "] had a error when serialize a msg, ignoring...";
            return;
        }

        zinx::ZinxPacket_LTD pack(id, payload);
        const auto& conn = client_->GetConnection();
        assert(conn);
        conn->Send(pack.GetData());
    }

    /// Simulates the player's action
    void AiRobotAction() {
        static std::default_random_engine random_engine;

        std::uniform_int_distribution random_generator(0, 1);
        int res = random_generator(random_engine);
        if (res) {
            // world message
            mmo::pb::Talk message;
            message.set_content(std::string("hello, i'm player#") += std::to_string(id_));
            DoPackAndSend(HANDLER_WORLD_CHAT_PACK_ID, &message);
        } else {
            // move
            float x = pos_.X; // length
            float z = pos_.Z; // width

            res = random_generator(random_engine);
            if (res) {
                std::uniform_int_distribution random_generator(0, 10);
                x -= random_generator(random_engine);
                z -= random_generator(random_engine);
            } else {
                std::uniform_int_distribution random_generator(0, 10);
                x += random_generator(random_engine);
                z += random_generator(random_engine);
            }

            // if (x > 410) {
            //     x = 410;
            // } else if (x < 85) {
            //     x = 85;
            // }

            // if (z > 400) {
			//     z = 400;
		    // } else if (z < 75) {
			//     z = 75;
		    // }

            float v = pos_.V;
            res = random_generator(random_engine);
            if (res) {
                v = 25;
            } else {
                v = 335;
            }

            mmo::pb::Position pos_message;
            pos_message.set_x(x);
            pos_message.set_y(pos_.Y);
            pos_message.set_z(z);
            pos_message.set_v(v);

            LOG_INFO << "the player[" << client_->GetName() << id_ << "] is moving: {"
                        << "x=" << x << ", y=" << pos_.Y
                        << ", z=" << z << ", v=" << v
                        << "}";

            DoPackAndSend(HANDLER_MOVE_PACK_ID, &pos_message);
        }
    }

private:
    int32_t id_ {-1};
    struct {
        float X;
        float Y;
        float Z;
        float V;
    } pos_;
    muduo::TcpClientPtr client_;
    zinx::ZinxDecoder_LTD decoder;
    muduo::detail::TimerId_t timerId_ {-1};  // for cancel the timed actions

    // for async, FIXME...
    std::promise<void> promise_;
    std::future<void> future_;
};


std::unique_ptr<Player> NewClient(muduo::EventLoop* loop, const std::string& ip, uint16_t port) {
    muduo::InetAddr server_addr(ip, port);
    muduo::TcpClientPtr client = muduo::CreateTcpClient(loop, server_addr, std::string("zinx-ai-client#"));
    std::unique_ptr<Player> player = std::make_unique<Player>(std::move(client));
    return player;
}

#define THREAD_NUM 1

std::atomic_uint16_t exited_client_num(0);

int main() {
    muduo::EventLoop loop;

    std::vector<std::unique_ptr<std::thread>> thread_pool;

    for (int i = 0; i < static_cast<int>(THREAD_NUM); i++) {
        thread_pool.emplace_back(std::make_unique<std::thread>([&loop]() {
            auto client = NewClient(&loop, "127.0.0.1", 8999U);
            client->Join();
            client->WaitPlayExit();

            // std::memory_order::memory_order_relaxed -> just need to ensure the property of atomic
            uint16_t cur_exited_num = exited_client_num.fetch_add(1, std::memory_order::memory_order_relaxed);
            if (cur_exited_num+1 == THREAD_NUM) {
                loop.Quit();
                LOG_INFO << "All the clients were exited.";
            }
        }));
    }

    loop.Loop();

    for (auto& t : thread_pool) {
        t->join();
    }
}