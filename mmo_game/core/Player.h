#if !defined(MMO_CORE_PLAYER_H)
#define MMO_CORE_PLAYER_H
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
class Player;       // forward declaration
class WorldManager; // forward declaration

/// Factory method
std::shared_ptr<Player> CreateNewPlayer(const zinx::ZinxConnectionPtr& conn, const Position& pos);

class Player : zinx::NonCopyable {
    friend std::shared_ptr<Player> CreateNewPlayer(const zinx::ZinxConnectionPtr& conn, const Position& pos);
    Player(int32_t pid, const zinx::ZinxConnectionPtr& conn, const Position& pos);
    
public:
    int32_t GetPid() const 
    { return pid_; }

    const Position& GetPosition() const
    { return pos_; }

    Position& GetPosition()
    { return pos_; }

    void SyncPid();

    /// 将自身位置广播至所有玩家客户端(包括自己)
    /// 将周围玩家信息同步到当前玩家客户端
    void SyncWithSurrounding(const mmo::WorldManager& wm);

    void WorldChat(const std::string& content, const mmo::WorldManager& wm);

    /// notify surrounding players that the current connection is offline
    void Offline(const mmo::WorldManager& wm);
    
    /// implement comparison predicate to adapt the container std::unorder_set
    /// check by muduo::TcpConnection instance 
    bool operator==(const Player& param) const {
        return conn_ == param.conn_;
    }
    
private:
    void SendMsgWithProtobuf(uint32_t id, google::protobuf::Message* msg);    
    std::vector<Player*> GetSurroundingPlayers(const WorldManager& wm) const;
    
private:
    int32_t pid_;
    zinx::ZinxConnectionPtr conn_;  // hold a connection handle
    Position pos_;
};

} // namespace mmo 

/// 通过hash value实现O(1)时间复杂度的查找，
/// 但在哈希碰撞（即两个不同的元素有相同的哈希值）的情况下，它还需要使用operator==来确保两个元素是否真正相等。
/// overload hash function，to calculate hash value of mmo::Player instance
namespace std {
    template <>
    struct hash<mmo::Player> {
        size_t operator()(const mmo::Player& player) const {
            /* calculate hash value by player`s pid */ 
            return hash<int32_t>()(player.GetPid());
        }
    };
}

#endif // MMO_CORE_PLAYER_H
