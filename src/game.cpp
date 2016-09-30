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
Game::refresh_map(const Entity* ignore_entity)
{
    static int count = 0;
    std::cout << "Refreshing: " << ++count << std::endl;
    for(auto& row : map) {
        for(auto& tile : row) {
            tile.passable = true;
        }
    }

    for(auto& entity : entities_) {
        if (ignore_entity == entity.get() ||
            !entity->has_component<Physics>() ||
            entity->get_component<Physics>()->is_passable()) {
            continue;
        }

        const auto* physics = entity->get_component<Physics>();

        auto box = physics->get_box();
        auto min_tile = get_tile_for(box.get_min_corner());
        auto tile_dimensions = to_tile_dimensions(box.get_dimensions());

        for (int y = 0; y < tile_dimensions.y; ++y) {
            for (int x = 0; x < tile_dimensions.x; ++x) {
                try {
                    map.at(y).at(x).passable = false;
                } catch (std::out_of_range&) {
                    // nothing to do here, entity dimensions exceed map bounds
                }
            }
        }
    }
}
