#include <mmo_game/core/AOI_Manager.h>
#include "AOI_Manager.h"

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
    target_grid.AddNewPlayer(pid);
}


void mmo::AOI_Manager::RemovePlayerFromGrid(int32_t pid, const Position& pos) {
    Grid& target_grid = GetGridByPosition(pos);
    target_grid.RemovePlayer(pid);
}
