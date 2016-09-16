#pragma once

#include <memory>

#include "entity.hpp"
#include "player.hpp"

class Game
{
public:
    Game(const Game&)           = delete;
    void operator=(const Game&) = delete;

    static Game& instance();

    inline const std::vector<std::unique_ptr<Entity>>& entities() const { return entities_; }
    inline std::vector<std::unique_ptr<Entity>>& entity_collection() { return entities_; }

    Player* add_player();
    inline Player* get_player() { return player_; };

private:
    Game();

    std::vector<std::unique_ptr<Entity>> entities_;
    Player* player_;
};
