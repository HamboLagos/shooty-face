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
        map_.clear();
        player_ = nullptr;
    }

    static Game& instance();

    inline const Entities& entities() const { return entities_; }
    inline Entities& entity_collection() { return entities_; }

    Player* add_player();
    inline Player* get_player() { return player_; };

    /** \brief (re)Initializes the tile map for this game. */
    void refresh_map();

    /** \brief Returns a Tile Map with the given entity marked as passable.
     *
     * Will ignore tiles that would be set as impassable by the given entity. */
    const TileMap get_map(const Entity* ignore_entity = nullptr);

    static sf::Vector2f get_tile_dimensions() { return sf::Vector2f(20.f, 20.f); }

    /** \brief Converts dimensions from float units to tile units. */
    static sf::Vector2i to_tile_dimensions(const sf::Vector2f dimensions)
    {
        return sf::Vector2i(std::ceil(dimensions.x / get_tile_dimensions().x),
                            std::ceil(dimensions.y / get_tile_dimensions().y));
    }

    /** \brief Returns the tile for the given position, where position is the TL (min) corner. */
    static sf::Vector2i get_tile_for(const sf::Vector2f& position)
    {
        return sf::Vector2i(position.x / get_tile_dimensions().x,
                            position.y / get_tile_dimensions().y);
    }

    /** \brief Returns the TL (min) corner position for the given tile. */
    static sf::Vector2f get_position_for(const sf::Vector2i& tile)
    {
        return sf::Vector2f(tile.x * get_tile_dimensions().x,
                            tile.y * get_tile_dimensions().y);
    }

    inline void set_timer(sf::Time time) { timer_ = time; }
    inline sf::Time get_timer() { return timer_; }

private:
    Game() :
        player_(nullptr)
    { }

    void initialize_map();

    Entities entities_;
    Player* player_;

    TileMap map_;

    sf::Time timer_;
};
