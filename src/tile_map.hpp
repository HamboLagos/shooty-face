#pragma once
/** \brief A Tile is either passable or not. */
struct Tile
{
    bool passable;

    inline bool operator==(const Tile tile) const
    {
        return passable == tile.passable;
    }

    inline bool operator!=(const Tile& tile) const
    {
        return !(*this == tile);
    }
};

/** \brief 2D vector of tiles, [y_vector][x_vector]. */
using TileMap = std::vector<std::vector<Tile>>;
