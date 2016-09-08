#include "bullet.hpp"
#include "utils.hpp"

Bullet::Bullet() :
    speed_(BULLET_SPEED),
    is_firing_(false)
{
    set_dimensions({10.f, 10.f});
    set_alive(false);
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
    sf::Vector2f trajectory = get_target() - get_position();
    float trajectory_length = sqrt(trajectory.x*trajectory.x + trajectory.y*trajectory.y);
    sf::Vector2f trajectory_direction = trajectory / trajectory_length;
    set_velocity(trajectory_direction * speed_);

    set_alive(true);
}

void
Bullet::update(sf::Time elapsed)
{
    if (is_dead()) {
        return;
    }

    float dt = elapsed.asSeconds();
    move(get_velocity() * dt);
}

void
Bullet::render()
{
    clear_renderings();

    if (is_dead()) {
        return;
    }

    graphic_.setRadius(get_extents().x);
    graphic_.setOrigin(get_extents());
    graphic_.setPosition(util::pixelate(get_position()));
    graphic_.setFillColor(sf::Color::Red);

    add_rendering(&graphic_);
}
