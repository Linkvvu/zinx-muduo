#if !defined(MMO_CORE_WORLD_MANAGER_H)
#define MMO_CORE_WORLD_MANAGER_H
#include <mmo_game/core/Player.h>
#include <mmo_game/core/AOI_Manager.h>
#include <unordered_map>
#include <shared_mutex>

namespace mmo {
class WorldManager;

/// global unique world manager handle
extern std::unique_ptr<WorldManager> GlobalWorldManager;

class WorldManager {
public:
    WorldManager(float world_length, float world_width, float cell_size)
        : aoiManager_(world_length, world_width, cell_size)
        { }

    void AddPlayer(const std::shared_ptr<Player>& player);
    void AddPlayer(std::shared_ptr<Player>&& player);
    void RemovePlayerByPid(int32_t pid);
    
    Player* GetPlayerByPid(int32_t pid) const {
        std::shared_lock<std::shared_mutex> guard(rw_mutex_);
        assert(players_.find(pid) != players_.end());
        return players_.at(pid).get();
    }

    const AOI_Manager& GetAoiManager() const
    { return aoiManager_; }

    const std::vector<Player*> GetAllPlayers() const;

private:
    /* AOI  module */
    AOI_Manager aoiManager_;
    /// The players in the current world
    std::unordered_map<int32_t, std::shared_ptr<Player>> players_ {};
    mutable std::shared_mutex rw_mutex_ {};
};

} // namespace mmo 

#endif // MMO_CORE_WORLD_MANAGER_H
