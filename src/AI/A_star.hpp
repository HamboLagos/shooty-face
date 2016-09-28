#pragma once

#include <vector>

class AStar
{
public:

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

    /** \brief Indices into a TileMap.
     *
     * Index members are declared in reverse order because they are used in reverse order, ie.
     * TileMap[y][x], not TileMap[x][y]. */
    struct Location
    {
        int y;
        int x;

        inline bool operator==(const Location& location) const
        {
            return
                y == location.y &&
                x == location.x;
        }

        inline bool operator!=(const Location& location) const
        {
            return !(*this == location);
        }
    };

    /** \brief [y_vector[x_vector]] tiles. */
    using TileMap = std::vector<std::vector<Tile>>;

    AStar()                 = delete;
    AStar(const AStar&)     = delete;
    AStar operator=(const AStar&) = delete;

    /** \brief Vector of locations to navigate a TileMap. */
    using Path = std::vector<Location>;

    struct Result
    {
        bool has_path; ///< true iff AStar::run() found a path through the map.
        Path path;     ///< vector of location indices to traverse the map
    };

    /** \brief Runs the AStar algorithm.
     *
     * When generating the map externally, the tiles must be at least the size of the entity. This
     * algo will not check for adjacent passable tiles.
     *
     * \param[in] target The target location.
     * \param[in] map Tile map
     *
     * \return Best path from start to end in the map.*/
    static Result run(const Location& start, const Location& end,
                                     const TileMap& map);
};
