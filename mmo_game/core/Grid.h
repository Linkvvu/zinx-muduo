#if !defined(MMO_CORE_GRID_H)
#define MMO_CORE_GRID_H

#include <zinx/inc/NonCopyable.h>
#include <mmo_game/core/Player.h>
#include <unordered_map>
#include <vector>
#include <mutex>

namespace mmo {

/// Represent a Grid in world map, all operations are thread safe on it 
class Grid : zinx::NonCopyable {
public:
    Grid() = default;
    
    std::vector<int32_t> GetAllPlayers() const;

    void AddNewPlayer(int32_t pid);
    void RemovePlayer(int32_t pid);
    
    /// for debug 
    bool HasPlayerByPid(int32_t pid) const
    { return players_.find(pid) != players_.end(); }
    
private:
    std::unordered_set<int32_t> players_;   // all players who in the grid 
    
    /// one grid per mutex, to optimize concurrent
    /// FIXME: should use rwlock ?
    mutable std::mutex mutex_;  
};

} // namespace mmo 


#endif // MMO_CORE_GRID_H
