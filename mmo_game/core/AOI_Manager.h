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
    std::vector<int32_t> GetSurroundingPlayersByPid(int32_t pid, const Position& pos) const;

private:
    Grid& GetGridByPosition(const Position& p)
    {
        const int row = static_cast<int>(p.Z / cellSize_);
        const int col = static_cast<int>(p.X / cellSize_);

        assert(IsVaildGrid(row, col));

        return worldMap_[row][col].operator*();
    }

    const Grid& GetGridByPosition(const Position& p) const
    {
        const int row = static_cast<int>(p.Z / cellSize_);
        const int col = static_cast<int>(p.X / cellSize_);

        assert(IsVaildGrid(row, col));

        return worldMap_[row][col].operator*();
    }

    bool IsVaildGrid(int row, int col) const 
    { return (row >= 0 && row < countY_) && (col >= 0 && col < countX_); }

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
