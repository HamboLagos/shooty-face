#include "player.hpp"

#include "collision.hpp"
#include "components/physics.hpp"
#include "game.hpp"
#include "utils.hpp"

Player::Player()
{
    set_component<Graphics>(std::make_unique<Graphics>(*this, *this));

    auto* physics = add_component<Physics>();
    physics->set_dimensions({20.f, 20.f});
    physics->set_move_speed(SPEED);
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
    auto* physics = get_component<Physics>();
    physics->set_velocity(direction * physics->get_move_speed());

    float min_percent_safe = 1.f;
    auto box = physics->get_box(elapsed.asSeconds());
    for(auto& entity : Game::instance().entities()) {
        if (!Collision::sanity_check(*this, *entity)) {
            continue;
        }

        auto entity_box = entity->get_component<Physics>()->get_box();

        if (Collision::broad_test(box, entity_box)) {
            float percent_safe = Collision::narrow_test(box, entity_box);

            if (percent_safe < 1.f) {
                if (percent_safe == 0.f) {
                    auto unpenetrate = Collision::get_penetration(box, entity_box);
                    entity->get_component<Physics>()->move(unpenetrate);
                }

                if (percent_safe < min_percent_safe) {
                    min_percent_safe = percent_safe;
                }
            }
        }
    }

    physics->update(elapsed.asSeconds() * min_percent_safe);
}

const Renderer::Renderings
Player::render()
{
    auto* physics = get_component<Physics>();

    graphic_.setSize(physics->get_dimensions());
    graphic_.setOrigin(physics->get_extents());
    graphic_.setPosition(util::pixelate(physics->get_position()));
    graphic_.setFillColor(sf::Color::Blue);

    Renderings renderings;
    renderings.reserve(1);
    renderings.push_back(&graphic_);

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
