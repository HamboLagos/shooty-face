#include "bullet.hpp"

Bullet::Bullet(sf::Vector2f initial_position, sf::Vector2f target, float speed)
{
    set_position(initial_position);

    sf::Vector2f trajectory = target - get_position();
    float trajectory_length = sqrt(trajectory.x*trajectory.x + trajectory.y*trajectory.y);
    sf::Vector2f trajectory_direction = trajectory / trajectory_length;
    set_velocity(trajectory_direction * speed);

    kill(); // bullets are animated when fire() is called
}

void
Bullet::fire()
{
    animate();
}

void
Bullet::update(sf::Time elapsed)
{
    float dt = elapsed.asSeconds();
    move(get_velocity() * dt);
}

const sf::Drawable&
Bullet::render()
{
    if (is_dead()) {
        graphic_.setFillColor(sf::Color::Transparent); // make it invisible
        return graphic_;
    }

    graphic_.setRadius(get_extents().x);
    graphic_.setOrigin(get_extents());
    graphic_.setPosition(get_position());
    graphic_.setFillColor(sf::Color::Red);
    return graphic_;
}
