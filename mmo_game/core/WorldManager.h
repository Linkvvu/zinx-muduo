#if !defined(MMO_WORLD_MANAGER_H)
#define MMO_WORLD_MANAGER_H
#include <mmo_game/core/Player.h>
#include <unordered_map>
#include <mutex>

namespace mmo {
class WorldManager;

/// global unique world manager handle
extern std::unique_ptr<WorldManager> GlobalWorldManager;

class WorldManager {
public:
    WorldManager() = default;

    void AddPlayer(const std::shared_ptr<Player>& player);
    void AddPlayer(std::shared_ptr<Player>&& player);
    void RemovePlayer(int32_t pid);
    

private:
    /* AOI  module */
    std::unordered_map<int32_t, std::shared_ptr<Player>> players_;
    /* mutex for players map*/
    std::mutex mutex_;
};

} // namespace mmo 

#endif // MMO_WORLD_MANAGER_H
