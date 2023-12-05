#include <muduo/base/Logging.h>
#include <mmo_game/core/AOI_Manager.h>
#include <cassert>

using namespace mmo;

AOI_Manager::AOI_Manager(float world_length, float world_width, float cell_size)
    : worldlength_(world_length)
    , worldWidth_(world_width)
    , cellSize_(cell_size)
    , countX_(static_cast<int>(world_length / cell_size))
    , countY_(static_cast<int>(world_width / cell_size))
    , worldMap_()
{
    // allocate the world map
    const_cast<Map_t&>(worldMap_).resize(countY_);
    for (int row = 0; row < countY_; row++) {
        const_cast<std::vector<GridPtr>&>(worldMap_[row]).resize(countX_);
        for (int col = 0; col < countX_; col++) {
            const_cast<GridPtr&>(worldMap_[row][col]).reset(new Grid());
        }
    }

    LOG_INFO << "The world map is initialized, detail: cell size="
            << cellSize_ << ", row number=" << countY_ << " column number=" << countX_;
}

std::vector<int32_t> AOI_Manager::GetSurroundingPlayersByPid(int32_t pid, const Position& pos) const {
    std::vector<int32_t> result;
    std::vector<Grid*> grids;

    int center_col_idx = GetColumnIndex(pos);
    int center_row_idx = GetRowIndex(pos);
    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            int cur_row = center_row_idx + x;
            int cur_col = center_col_idx + y;
            if (IsVaildGrid(cur_row, cur_col)) {
                Grid* cur_grid = worldMap_[cur_row][cur_col].get();
                assert(cur_grid != nullptr);
                grids.push_back(cur_grid);
            }
        }
    }

    for (const Grid* g : grids) {
        const auto& players = g->GetAllPlayers();
        result.insert(result.end(), players.begin(), players.end());
    }

    return result;
}

Grid& AOI_Manager::GetGridByPosition(const Position& p) const {
    const int row = GetRowIndex(p);
    const int col = GetColumnIndex(p);
    assert(IsVaildGrid(row, col));
    return worldMap_[row][col].operator*();
}
