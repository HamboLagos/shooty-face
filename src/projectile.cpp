#include "projectile.hpp"

void
Projectile::update(sf::Time elapsed)
{
    float dt = elapsed.asSeconds();
    move(get_velocity() * dt);
}

const sf::Drawable&
Projectile::render()
{
    graphic_.setRadius(get_extents().x);
    graphic_.setOrigin(get_extents());
    graphic_.setPosition(get_position());
    graphic_.setFillColor(sf::Color::Red);
    return graphic_;
}
