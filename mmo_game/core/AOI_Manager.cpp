#include <mmo_game/core/AOI_Manager.h>
#include <cassert>

mmo::AOI_Manager::AOI_Manager(float world_length, float world_width, float cell_size)
    : worldlength_(world_length)
    , worldWidth_(world_width)
    , cellSize_(cell_size)
    , countX_(static_cast<int>(world_length / cell_size))
    , countY_(static_cast<int>(world_width / cell_size))
    , worldMap_()
{
    // allocate the world map
    worldMap_.resize(countY_);
    for (int row = 0; row < countY_; row++) {
        worldMap_[row].resize(countX_);
        for (int col = 0; col < countX_; col++) {
            worldMap_[row][col].reset(new Grid());
        }
    }

    std::clog << "The world map is initialized, detail: cell size="
            << cellSize_ << " X grid num=" << countX_ << " Y grid num=" << countY_ << std::endl;
}

void mmo::AOI_Manager::AddPlayerToGrid(int32_t pid, const Position& pos) {
    Grid& target_grid = GetGridByPosition(pos);
    assert(!target_grid.HasPlayerByPid(pid));
    target_grid.AddNewPlayer(pid);
}


void mmo::AOI_Manager::RemovePlayerFromGrid(int32_t pid, const Position& pos) {
    Grid& target_grid = GetGridByPosition(pos);
    assert(target_grid.HasPlayerByPid(pid));
    target_grid.RemovePlayer(pid);
}

std::vector<int32_t> mmo::AOI_Manager::GetSurroundingPlayersByPid(int32_t pid, const Position& pos) const {
    // const Grid& target_grid = GetGridByPosition(pos);
    // assert(target_grid.HasPlayerByPid(pid));
    std::vector<int32_t> result;

    int grid_X = static_cast<int>(pos.X / cellSize_);
    int grid_Y = static_cast<int>(pos.Z / cellSize_);
    const Grid* cur_grip = nullptr;
    for (int j = -1; j <= 1; ++j) {
        for (int i = -1; i <= 1; ++i) {
            int cur_row = grid_Y + i;
            int cur_col = grid_X + j;
            if (IsVaildGrid(cur_row, cur_col)) {
                cur_grip = worldMap_[cur_row][cur_col].get();
                std::vector<int32_t> pids = cur_grip->GetAllPlayers();
                result.insert(result.end(), pids.begin(), pids.end());
            }
        }
    }

    return result;
}
