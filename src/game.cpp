#include <iostream>
#include <stdexcept>

#include "game.hpp"
#include "components/physics.hpp"

Game&
Game::instance()
{
    static Game game;
    return game;
}

Player*
Game::add_player()
{
    if (player_ != nullptr) {
        entities_.erase(
            std::find_if(entities_.begin(), entities_.end(),
                         [this](auto& entity) { return entity.get() == player_; }));
    }

    player_ = new Player();
    entities_.emplace_back(player_);

    return player_;
}

void
Game::refresh_map()
{
    initialize_map();

    for(auto& entity : entities_) {
        if (player_ == entity.get()           ||
            !entity->has_component<Physics>() ||
            entity->get_component<Physics>()->is_passable()) {
            continue;
        }

        const auto* physics = entity->get_component<Physics>();
        auto box = physics->get_box();
        auto min_tile = get_tile_for(box.get_min_corner());
        auto max_tile = get_tile_for(box.get_max_corner());
        auto tile_dimensions = sf::Vector2i{1, 1} + (max_tile - min_tile);

        for (int y = 0; y < tile_dimensions.y; ++y) {
            for (int x = 0; x < tile_dimensions.x; ++x) {
                try {
                    map_.at(min_tile.y + y).at(min_tile.x + x).passable = false;
                } catch (std::out_of_range&) {
                    // nothing to do here, entity dimensions exceed map_ bounds
                }
            }
        }
    }
}

const TileMap
Game::get_map(const Entity* ignore_entity)
{
    TileMap copy = map_;
    if (ignore_entity && ignore_entity->has_component<Physics>()) {

        const auto* physics = ignore_entity->get_component<Physics>();
        auto box = physics->get_box();
        auto min_tile = get_tile_for(box.get_min_corner());
        auto max_tile = get_tile_for(box.get_max_corner());
        auto tile_dimensions = sf::Vector2i{1, 1} + (max_tile - min_tile);

        for (int y = 0; y < tile_dimensions.y; ++y) {
            for (int x = 0; x < tile_dimensions.x; ++x) {
                try {
                    copy.at(min_tile.y + y).at(min_tile.x + x).passable = true;
                } catch (std::out_of_range&) {
                    // nothing to do here, entity dimensions exceed map_ bounds
                }
            }
        }
    }

    return copy;
}

void
Game::initialize_map()
{
    const auto tiles_high = std::ceil(WINDOW_HEIGHT / get_tile_dimensions().y);
    const auto tiles_wide = std::ceil(WINDOW_WIDTH / get_tile_dimensions().x);

    map_.clear();
    map_.reserve(tiles_high);
    for (int y = 0; y < tiles_high; ++y) {
        map_.emplace_back(tiles_wide, Tile{true});
    }
}
