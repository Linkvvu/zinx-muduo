#include <mmo_game/core/Grid.h>

using namespace mmo;

void Grid::AddNewPlayer(int32_t pid) {
    std::lock_guard<std::mutex> guard(mutex_);
    assert(players_.find(pid) == players_.end());
    players_.insert(pid);
}

void Grid::RemovePlayer(int32_t pid) {
    std::lock_guard<std::mutex> guard(mutex_);
    assert(players_.find(pid) != players_.end());
    size_t ret = players_.erase(pid);
    assert(ret == 1);
}

/// time complexity: O(N) 
std::vector<int32_t> Grid::GetAllPlayers() const {
    
}
