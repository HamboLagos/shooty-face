#include "player.hpp"
#include "utils.hpp"
#include "collision.hpp"
#include "game.hpp"

#include <cmath>

#include <iostream>

Player::Player() :
    gun_(*this)
{
    auto* graphics = add_component<Graphics>();
    graphics->on_render(std::bind(&Player::render, this));
}

void
Player::update(sf::Time elapsed)
{
    float dx = 0.f;
    float dy = 0.f;

    if (is_moving.left && !is_moving.right) {
        dx = -1.f;
    } else if (is_moving.right && !is_moving.left) {
        dx = 1.f;
    }

    if (is_moving.up && !is_moving.down) {
        dy = -1.f;
    } else if (is_moving.down && !is_moving.up) {
        dy = 1.f;
    }

    auto direction = util::direction({dx, dy});
    set_velocity(direction * SPEED);


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
                auto dt_safe = Collision::narrow_test(this->get_box(elapsed), entity->get_box());

                if (dt_safe == 0.f) {
                    auto unpenetrate = Collision::get_penetration(this->get_box(), entity->get_box());
                    entity->move(unpenetrate);
                    resolved = false;
                    continue;
                } else if (dt_safe < dt) {
                    dt = dt_safe;
                }
            }
        }
    }

    move(get_velocity() * dt);

    gun_.update(elapsed);
}

const Graphics::Renderings
Player::render()
{
    graphic_.setSize(get_dimensions());
    graphic_.setOrigin(get_extents());
    graphic_.setPosition(util::pixelate(get_position()));
    graphic_.setFillColor(sf::Color::Blue);

    const auto& gun_renderings = gun_.render();

    Graphics::Renderings renderings;
    renderings.reserve(1 + gun_renderings.size());
    renderings.push_back(&graphic_);
    renderings.insert(renderings.end(), gun_renderings.begin(), gun_renderings.end());

    return std::move(renderings);
}

void
Player::start_move(Direction direction)
{
    is_moving.set(true, direction);
}

void
Player::stop_move(Direction direction)
{
    is_moving.set(false, direction);
}

void
Player::MovementDirections::set(bool is_moving, Direction direction)
{
    switch (direction) {

    case Direction::UP:
        up = is_moving;
        break;

    case Direction::LEFT:
        left = is_moving;
        break;

    case Direction::DOWN:
        down = is_moving;
        break;

    case Direction::RIGHT:
        right = is_moving;
        break;

    default:
        break;
    }
}
