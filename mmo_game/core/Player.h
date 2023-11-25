#if !defined(MMO_PLAYER_H)
#define MMO_PLAYER_H
#include <google/protobuf/message.h>
#include <zinx/inc/ZConnection.h>
#include <zinx/inc/NonCopyable.h>
#include <memory>

namespace mmo {

struct Position
{
    float X;    // length
    float Y;    // height
    float Z;    // width
    float V;    // angle
};
class Player;   // forward declaration

/// Factory method
std::shared_ptr<Player> CreateNewPlayer(const zinx::ZinxConnectionPtr& conn, const Position& pos);

class Player : zinx::NonCopyable {
    friend std::shared_ptr<Player> CreateNewPlayer(const zinx::ZinxConnectionPtr& conn, const Position& pos);
    Player(int32_t pid, const zinx::ZinxConnectionPtr& conn, const Position& pos);
    
public:
    int32_t GetPid() const 
    { return pid_; }

    void SyncPid();

    void BroadcastInitialPos();
    
private:
    void SendMsgWithProtobuf(uint32_t id, google::protobuf::Message* msg);    
    
private:
    int32_t pid_;
    zinx::ZinxConnectionPtr conn_;  // hold a connection handle
    Position pos_;
};

} // namespace mmo 


#endif // MMO_PLAYER_H
