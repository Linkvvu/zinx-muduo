#include <mmo_game/core/WorldManager.h>

using namespace mmo;

std::unique_ptr<WorldManager> mmo::GlobalWorldManager = std::make_unique<WorldManager>();

void WorldManager::AddPlayer(const std::shared_ptr<Player>& player) {
    std::lock_guard<std::mutex> guard(mutex_);

    assert(players_.find(player->GetPid()) == players_.end());

    players_[player->GetPid()] = player;
}

void WorldManager::AddPlayer(std::shared_ptr<Player>&& player) {
    std::lock_guard<std::mutex> guard(mutex_);

    assert(players_.find(player->GetPid()) == players_.end());

    players_[player->GetPid()] = std::move(player);
}

void WorldManager::RemovePlayer(int32_t pid) {
    std::lock_guard<std::mutex> guard(mutex_);

    assert(players_.find(pid) != players_.end());
    
    size_t present = players_.erase(pid);
    assert(present = 1);
}