#if !defined(MMO_CORE_AOI_MANAGER_H)
#define MMO_CORE_AOI_MANAGER_H

#include <mmo_game/core/Grid.h>
#include <array>
namespace mmo {

/// FIXME: 解耦合 game map & AOI-Manager
/// Manages a constant world map
/// All member is constant, so AOI_Manager is thread safe
class AOI_Manager : zinx::NonCopyable {
public:
    AOI_Manager(float world_length, float world_width, float cell_size);
    
    void AddPlayerToGrid(int32_t pid, const Position& pos) const {
        Grid& target_grid = GetGridByPosition(pos);
        AddPlayerToGrid(pid, target_grid);
    }

    void AddPlayerToGrid(int32_t pid, Grid& target_grid) const {
        target_grid.AddNewPlayer(pid);
    }

    void RemovePlayerFromGrid(int32_t pid, const Position& pos) const {
        Grid& target_grid = GetGridByPosition(pos);
        RemovePlayerFromGrid(pid, target_grid);
    }

    void RemovePlayerFromGrid(int32_t pid, Grid& target_grid) const {
        target_grid.RemovePlayer(pid);
    }

    /// Get surrounding all players`s PID of given player
    std::vector<int32_t> GetSurroundingPlayersByPid(int32_t pid, const Position& pos) const;

    /// Get surrounding all players`s PID of given player
    std::vector<int32_t> GetSurroundingPlayersByPid(const Player* p) const
    { return GetSurroundingPlayersByPid(p->GetPid(), p->GetPosition()); }

    std::vector<const Grid*> GetSudokuByPosition(const Position& pos) const;

    Grid& GetGridByPosition(const Position& p) const;

private:
    int GetColumnIndex(const Position& p) const

    { return static_cast<int>(p.X / cellSize_); }

    int GetRowIndex(const Position& p) const
    { return static_cast<int>(p.Z / cellSize_); }

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
    const Map_t worldMap_;  // Two-dimensional map
};


} // namespace mmo 

#endif // MMO_CORE_AOI_MANAGER_H
