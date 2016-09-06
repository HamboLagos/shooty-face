#include "enemy.hpp"

void
Enemy::update(sf::Time elapsed)
{
    if (is_dead()) {
        return;
    }

    float dt = elapsed.asSeconds();
    move(get_velocity() * dt);
}

const sf::Drawable&
Enemy::render()
{
    graphic_.setSize(get_dimensions());
    graphic_.setOrigin(get_extents());
    graphic_.setPosition(get_position());
    graphic_.setFillColor(sf::Color::Green);

    graphic_.setOutlineColor(sf::Color::Red);
    graphic_.setOutlineThickness((get_health()/100.f - 1) * get_dimensions().x/2.f);
    return graphic_;
}

void
Enemy::on_death()
{
    kill();
}
