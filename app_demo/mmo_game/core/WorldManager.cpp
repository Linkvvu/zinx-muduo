#include <mmo_game/core/WorldManager.h>
#include <mutex>    // for unique_lock

using namespace mmo;

std::unique_ptr<WorldManager> mmo::GlobalWorldManager {};

void mmo::initGlobalWorldManager() {
    mmo::GlobalWorldManager = std::make_unique<WorldManager>(
        400.0F, 400.0F, 20.0F
    );
}

void WorldManager::AddPlayer(const std::shared_ptr<Player>& player) {
    {
        std::unique_lock<std::shared_mutex> guard(rw_mutex_);
        assert(!HasPlayer(player->GetPid()));
        players_[player->GetPid()] = player;    
    }

    aoiManager_.AddPlayerToGrid(player->GetPid(), player->GetPosition());
}

void WorldManager::AddPlayer(std::shared_ptr<Player>&& player) {
    const uint32_t pid = player->GetPid();
    const Position& pos = player->GetPosition();
    {
        std::unique_lock<std::shared_mutex> guard(rw_mutex_);
        assert(!HasPlayer(player->GetPid()));
        players_[player->GetPid()] = std::move(player);    
    }

    aoiManager_.AddPlayerToGrid(pid, pos);
}

void WorldManager::RemovePlayerByPid(int32_t pid) {
    std::unique_lock<std::shared_mutex> guard(rw_mutex_);
    
    assert(HasPlayer(pid));
    std::shared_ptr<Player>& target_player = players_[pid];

    // notify that the target player is offline to aoi_manager
    const Position& pos_of_target_player = target_player->GetPosition();
    aoiManager_.RemovePlayerFromGrid(pid, pos_of_target_player);

    // remove player instance
    size_t ret = players_.erase(pid);
    assert(ret == 1);
}

std::vector<PlayerPtr> WorldManager::GetAllPlayers() const {
    std::shared_lock<std::shared_mutex> guard(rw_mutex_);
    
    std::vector<PlayerPtr> all_players;
    all_players.reserve(players_.size());
    for (const auto& pair : players_) {
        all_players.push_back(pair.second);
    }
    return all_players;
}

std::vector<PlayerPtr> mmo::WorldManager::GetAllPlayers(const std::vector<const Grid*>& grids) const {
    std::shared_lock<std::shared_mutex> guard(rw_mutex_);

    std::vector<PlayerPtr> result;
    for (const Grid* g : grids) {
        const std::vector<int32_t> pids = g->GetAllPlayers();
        for (int32_t pid : pids) {
            result.push_back(GetPlayerByPid(pid, false));
        }
    }
    return result;
}

std::vector<PlayerPtr> WorldManager::GetSurroundingPlayers(int32_t pid) const {
    std::shared_lock<std::shared_mutex> guard(rw_mutex_);

    PlayerPtr target_player = GetPlayerByPid(pid, false);
    std::vector<int32_t> players = aoiManager_.GetSurroundingPlayersByPid(target_player);

    std::vector<PlayerPtr> result;
    result.reserve(players.size());
    
    for (int32_t pid : players) {
        result.push_back(GetPlayerByPid(pid, false));
    }
    
    return result;
}

