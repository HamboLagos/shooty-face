#pragma once

#include <vector>
#include <memory>

#include "entity.hpp"
#include "player.hpp"
#include "tile_map.hpp"

class Game
{
public:
    static constexpr float WINDOW_WIDTH = 1200.f;
    static constexpr float WINDOW_HEIGHT = 1200.f;

    using Entities = std::vector<std::unique_ptr<Entity>>;

    Game(const Game&)           = delete;
    void operator=(const Game&) = delete;

    void reset()
    {
        entities_.clear();
        player_ = nullptr;
    }

    static Game& instance();

    inline const Entities& entities() const { return entities_; }
    inline Entities& entity_collection() { return entities_; }

    Player* add_player();
    inline Player* get_player() { return player_; };


    /** \brief Refreshes and returns a reference to the the tile map.
     *
     * Will ignore tiles that would be set as impassable by the given entity. */
    inline const TileMap& get_map(const Entity* ignore_entity = nullptr)
    {
        refresh_map(ignore_entity);
        return map;
    }

    inline sf::Vector2f get_tile_dimensions() const { return sf::Vector2f(10.f, 10.f); }

    /** \brief Returns the tile for the given position, where position is the TL (min) corner. */
    inline sf::Vector2i get_tile_for(const sf::Vector2f& position) const
    {
        return sf::Vector2i(position.x / get_tile_dimensions().x,
                            position.y / get_tile_dimensions().y);
    }

    /** \brief Returns the TL (min) corner position for the given tile. */
    inline sf::Vector2f get_position_for(const sf::Vector2i& tile) const
    {
        return sf::Vector2f(tile.x * get_tile_dimensions().x,
                            tile.y * get_tile_dimensions().y);
    }

    inline sf::Vector2i to_tile_dimensions(const sf::Vector2f dimensions) const
    {
        return sf::Vector2i(std::ceil(dimensions.x / get_tile_dimensions().x),
                            std::ceil(dimensions.y / get_tile_dimensions().y));
    }

private:
    Game() :
        player_(nullptr)
    {
        const auto tiles_high = std::ceil(WINDOW_HEIGHT / get_tile_dimensions().y);
        const auto tiles_wide = std::ceil(WINDOW_WIDTH / get_tile_dimensions().x);

        map.reserve(tiles_high);
        for (int y = 0; y < tiles_high; ++y) {
            map.push_back(std::vector<Tile>());
            map.back().reserve(tiles_wide);
            for (int x = 0; x < tiles_wide; ++x) {
                map.back().push_back(Tile{true});
            }
        }
    }

    Entities entities_;
    Player* player_;

    TileMap map;

    void refresh_map(const Entity* ignore_entity);
};
