#if !defined(MMO_CORE_WORLD_MANAGER_H)
#define MMO_CORE_WORLD_MANAGER_H
#include <mmo_game/core/Player.h>
#include <mmo_game/core/AOI_Manager.h>
#include <unordered_map>
#include <shared_mutex>
#include <vector>

namespace mmo {
class WorldManager;

/// global unique world manager handle
/// @note must use after mmo::initGlobalWorldManager;
extern std::unique_ptr<WorldManager> GlobalWorldManager;
/// initialize global world manager instance
extern void initGlobalWorldManager();

class WorldManager {
public:
    WorldManager(float world_length, float world_width, float cell_size)
        : aoiManager_(world_length, world_width, cell_size)
        { }

    void AddPlayer(const std::shared_ptr<Player>& player);
    void AddPlayer(std::shared_ptr<Player>&& player);
    void RemovePlayerByPid(int32_t pid);

    const AOI_Manager& GetAoiManager() const
    { return aoiManager_; }

    Player* GetPlayerByPid(int32_t pid, bool lock = true) const {
        if (lock) {
            std::shared_lock<std::shared_mutex> guard(rw_mutex_);
            assert(HasPlayer(pid));
            return players_.at(pid).get();
        } else {
            assert(HasPlayer(pid));
            return players_.at(pid).get();
        }
    }

    /// Get all players of the current world 
    std::vector<Player*> GetAllPlayers() const;

    /// Get all players of the given grids 
    std::vector<Player*> GetAllPlayers(const std::vector<const Grid*>& grids) const;

    std::vector<Player*> GetSurroundingPlayers(int32_t pid) const;

private:
    bool HasPlayer(int32_t pid) const 
    { return players_.find(pid) != players_.end(); }

private:
    // AOI  module
    AOI_Manager aoiManager_;

    /// All the players in the current world
    std::unordered_map<int32_t, std::shared_ptr<Player>> players_ {};
    mutable std::shared_mutex rw_mutex_ {};
};

} // namespace mmo 

#endif // MMO_CORE_WORLD_MANAGER_H
