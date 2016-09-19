#include "game.hpp"

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
