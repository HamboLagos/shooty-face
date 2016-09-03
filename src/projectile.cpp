#include "projectile.hpp"

sf::Vector2f
Projectile::get_position() const
{
    return position_;
}

void
Projectile::set_position(sf::Vector2f position)
{
    position_ = position;
}

sf::Vector2f
Projectile::get_velocity() const
{
    return velocity_;
}

void
Projectile::set_velocity(sf::Vector2f velocity)
{
    velocity_ = velocity;
}

void
Projectile::update()
{
    position_ += velocity_;
}

void
Projectile::kill()
{
    alive_ = false;
}

AABB
Projectile::get_AABB()
{
    return AABB(position_, dimensions_);
}

const sf::CircleShape
Projectile::render()
{
    graphic_.setPosition(position_);
    return graphic_;
}
