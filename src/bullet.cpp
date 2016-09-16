#include "bullet.hpp"
#include "utils.hpp"
#include "collision.hpp"
#include "game.hpp"
#include "health.hpp"

Bullet::Bullet() :
    is_firing_(false)
{
    set_dimensions({10.f, 10.f});
}

Projectile*
Bullet::create_projectile()
{
    if (!is_firing_ || firing_pin_.getElapsedTime().asSeconds() >= 1.f/FIRE_RATE) {
        is_firing_ = true;
        firing_pin_.restart();

        return new Bullet;
    }

    return nullptr;
}

void
Bullet::reload()
{
    is_firing_ = false;
}

void
Bullet::fire()
{
    auto trajectory = get_target() - get_position();
    set_velocity(util::direction(trajectory) * SPEED);
}

void
Bullet::update(sf::Time elapsed)
{
    if (is_dead()) {
        return;
    }

    float dt = elapsed.asSeconds();
    move(get_velocity() * dt);

    for(auto& entity : Game::instance().entities()) {
        if (entity->is_passable() || entity.get() == this ||
            entity.get() == Game::instance().get_player()) {
            continue;
        }

        if (Collision::broad_test(get_box(), entity->get_box())) {
            if (entity->has_component<Health>()) {
                entity->get_component<Health>()->damage(5.f);
            }

            kill();
        }
    }
}

const sf::Drawable*
Bullet::render()
{
    graphic_.setRadius(get_extents().x);
    graphic_.setOrigin(get_extents());
    graphic_.setPosition(util::pixelate(get_position()));
    graphic_.setFillColor(sf::Color::Red);

    return &graphic_;
}
