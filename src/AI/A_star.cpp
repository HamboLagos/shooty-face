#include <stdexcept>

#include "A_star.hpp"

AStar::Result
AStar::run(const Location& start, const Location& end, const TileMap& map)
{
    try {
        // check start and end in the map
        map.at(start.y).at(start.x);
        map.at(end.y).at(end.x);

        if (start == end) {
            return Result{true, Path{start}};
        }

        bool vertical = false;
        if ((vertical = start.x == end.x) || start.y == end.y) {
            Path linear_path;
            Location next_step = start;
            bool has_path = true;

            while (next_step != end) {
                linear_path.push_back(next_step);

                /// TODO could decrease too
                vertical ? ++next_step.y : ++next_step.x;

                if (!map.at(next_step.y).at(next_step.x).passable) {
                    has_path = false;
                    break;
                }
            }
            linear_path.push_back(end);

            return Result{has_path, linear_path};
        }

    } catch (std::out_of_range&) {
        // we tried to index to a location outside the map range
        return Result{false, Path()};
    }



    return Result{true, Path()};
}
