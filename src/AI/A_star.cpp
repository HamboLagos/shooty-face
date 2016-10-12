#include <iostream>

#include <stdexcept>
#include <algorithm>

#include <SFML/System/Clock.hpp>

#include "A_star.hpp"
#include "utils.hpp"
#include "game.hpp"

AStar::CostMap AStar::cost_so_far_;
AStar::FromMap AStar::came_from_;
AStar::PQ AStar::frontier_;

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

    if (!is_reachable(end, dimensions, map)) {
        return Result{false, Path()};
    }

    if (start == end) {
        return Result{true, Path{start}};
    }

    cost_so_far_.clear();
    came_from_.clear();
    while(!frontier_.empty()) { frontier_.pop(); }

    cost_so_far_[start] = 0.f;
    came_from_[start] = start;
    frontier_.emplace(0.f, start);

    int loops = 0;
    int max_size = 0;
    sf::Clock clock;
    while(!frontier_.empty())
    {
        ++loops;
        auto current = frontier_.top().second;
        frontier_.pop();

        if (current == end) {
            break;
        }

        const auto neighbors = get_neighbors(current, dimensions, map);
        for(const auto& next : neighbors) {
            float cost = cost_so_far_[current] +
                util::length(Game::get_position_for(current) -
                             Game::get_position_for(next));
            if (!cost_so_far_.count(next) || cost < cost_so_far_[next]) {
                cost_so_far_[next] = cost;
                came_from_[next] = current;
                float priority = cost +
                    util::length(Game::get_position_for(end) -
                                 Game::get_position_for(next));
                frontier_.emplace(priority, next);

                if (frontier_.size() > max_size) {
                    max_size = frontier_.size();
                }
            }
        }
    }
    Game::instance().set_timer(clock.getElapsedTime());
    std::cout << "loops: " << loops << " size: " << max_size << std::endl;

    // we didn't find a way through
    if (!came_from_.count(end)) {
        return {false, Path()};
    }

    Path path;
    auto current = end;
    while (current != start) {
        path.push_back(current);
        current = came_from_[current];
    }
    path.push_back(start);

    std::reverse(path.begin(), path.end());

    return Result{true, path};
}
