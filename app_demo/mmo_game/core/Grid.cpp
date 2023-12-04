#include <mmo_game/core/Grid.h>

using namespace mmo;

void Grid::AddNewPlayer(int32_t pid) {
    std::lock_guard<std::mutex> guard(mutex_);
    assert(!HasPlayerByPid(pid));
    players_.insert(pid);
}

void Grid::RemovePlayer(int32_t pid) {
    std::lock_guard<std::mutex> guard(mutex_);
    assert(HasPlayerByPid(pid));
    size_t ret = players_.erase(pid);
    assert(ret == 1);
}

/// time complexity: O(N) 
std::vector<int32_t> Grid::GetAllPlayers() const {
    std::lock_guard<std::mutex> guard(mutex_);
    return std::vector<int32_t>(players_.begin(), players_.end());
}
