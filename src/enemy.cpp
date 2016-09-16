#include <functional>
#include <iostream>

#include "collision.hpp"
#include "enemy.hpp"
#include "game.hpp"
#include "health.hpp"
#include "utils.hpp"
#include "graphics.hpp"

Enemy::Enemy() :
    is_fast_(false)
{
    auto* health = add_component<Health>();
    health->on_death(std::bind(&Enemy::on_death, this));

    auto* graphics = add_component<Graphics>();
    graphics->on_render(std::bind(&Enemy::render, this));
}

void
Enemy::update(sf::Time elapsed)
{
    if (is_dead()) {
        return;
    }

    auto* player = Game::instance().get_player();
    if (player == nullptr) {
        return;
    }

    auto direction = util::direction(player->get_position() - get_position());
    set_velocity(direction * (is_fast_ ? FAST_SPEED : SLOW_SPEED));

    float dt = elapsed.asSeconds();
    bool resolved = false;
    for (int loops = 0; !resolved && loops < 8; ++loops) {
        dt = elapsed.asSeconds();
        resolved = true;

        for(auto& entity : Game::instance().entities()) {
            if (entity->is_passable() || entity.get() == this) {
                continue;
            }

            if (Collision::broad_test(this->get_box(elapsed), entity->get_box())) {
                auto dt_safe = Collision::narrow_test(this->get_box(), entity->get_box());

                if (dt_safe == 0.f) {
                    auto unpenetrate = -Collision::get_penetration(this->get_box(elapsed), entity->get_box());
                    move(unpenetrate);
                    resolved = false;
                    continue;
                } else if (dt_safe < dt) {
                    dt = dt_safe;
                }
            }
        }
    }

    move(get_velocity() * dt);
}

const Graphics::Renderings
Enemy::render()
{
    graphic_.setSize(get_dimensions());
    graphic_.setOrigin(get_extents());
    graphic_.setPosition(util::pixelate(get_position()));
    graphic_.setFillColor(sf::Color::Green);
    graphic_.setOutlineThickness(-1.f);
    graphic_.setOutlineColor(sf::Color::Black);

    Graphics::Renderings renderings;
    renderings.reserve(1);
    renderings.push_back(&graphic_);

    return std::move(renderings);

    /* add_renderings(Health::render(get_position(), -(get_extents().y + 5.f))); */
}

void
Enemy::on_death()
{
    std::cout << "I Die!" << std::endl;
    set_alive(false);
}
