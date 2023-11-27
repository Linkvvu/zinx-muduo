#if !defined(MMO_CORE_AOI_MANAGER_H)
#define MMO_CORE_AOI_MANAGER_H

#include <zinx/inc/NonCopyable.h>
#include <mmo_game/core/Grid.h>

namespace mmo {

class AOI_Manager : zinx::NonCopyable {
public:
    AOI_Manager(float world_length, float world_width, float cell_size);
    void AddPlayerToGrid(int32_t pid, const Position& pos);
    void RemovePlayerFromGrid(int32_t pid, const Position& pos);

private:
    Grid& GetGridByPosition(const Position& p)
    {
        const int row = static_cast<int>(p.Z / cellSize_);
        const int col = static_cast<int>(p.X / cellSize_);

        assert(row < countY_ && row >= 0);
        assert(col < countX_ && col >= 0);

        return worldMap_[row][col].operator*();
    }

private:
    const float worldlength_;
    const float worldWidth_;
    const float cellSize_;
    const int countX_;  // number of grids in a row
    const int countY_;  // number of grids in a column

    using GridPtr = std::unique_ptr<mmo::Grid>;
    using Map_t = std::vector<std::vector<GridPtr>>;
    Map_t worldMap_;  // Two-dimensional map
};


} // namespace mmo 

#endif // MMO_CORE_AOI_MANAGER_H
