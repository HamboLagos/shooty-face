#include <stdexcept>
#include <algorithm>

#include "A_star.hpp"
#include "utils.hpp"

bool
is_reachable(const sf::Vector2i& location, const sf::Vector2i& dimensions,
             const TileMap& map)
{
    // subtract one from each dimension, because we assume the tile directly at location is
    // always passable
    for (int y = 0; y < dimensions.y; ++y) {
        for (int x = 0; x < dimensions.x; ++x) {
            try {
                if (!map.at(location.y + y).at(location.x + x).passable) {
                    return false;
                }
            } catch (std::out_of_range&) {
                // we tried to index to a location outside the map range
                // by definition, a tile outside the map range is impassable
                return false;
            }
        }
    }

    return true;
}

static const std::vector<sf::Vector2i>
get_neighbors(const sf::Vector2i& location, const sf::Vector2i& dimensions,
              const TileMap& map)
{
    static const std::vector<sf::Vector2i> directions = {{0, -1}, {-1, 0}, {0, 1}, {1, 0}};

    std::vector<sf::Vector2i> neighbors;

    for(const auto& direction : directions) {
        if (is_reachable(location + direction, dimensions, map)) {
            neighbors.emplace_back(location + direction);
        }
    }

    return std::move(neighbors);
}

AStar::Result
AStar::run(const sf::Vector2i& start, const sf::Vector2i& end,
           const sf::Vector2i& dimensions, const TileMap& map)
{
    // validate the start and end positions
    if (!is_reachable(start, dimensions, map) || !is_reachable(end, dimensions, map)) {
        return Result{false, Path()};
    }

    if (start == end) {
        return Result{true, Path{start}};
    }

    auto cost_so_far = CostMap();
    cost_so_far[start] = 0.f;

    auto came_from = FromMap();
    came_from[start] = start;

    auto frontier = PQ();
    frontier.emplace(0.f, start);

    while(!frontier.empty())
    {
        auto current = frontier.top().second;
        frontier.pop();

        if (current == end) {
            break;
        }

        const auto neighbors = get_neighbors(current, dimensions, map);
        for(const auto& next : neighbors) {
            float cost = cost_so_far[current] + util::length(sf::Vector2f(current - next));
            if (!cost_so_far.count(next) || cost < cost_so_far[next]) {
                cost_so_far[next] = cost;
                came_from[next] = current;
                float priority = cost + util::length(sf::Vector2f(end - next));
                frontier.emplace(priority, next);
            }
        }
    }

    // we didn't find a way through
    if (!came_from.count(end)) {
        return {false, Path()};
    }

    Path path;
    auto current = end;
    while (current != start) {
        path.push_back(current);
        current = came_from[current];
    }
    path.push_back(start);

    std::reverse(path.begin(), path.end());

    return Result{true, path};
}
