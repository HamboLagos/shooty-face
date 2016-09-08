#include "enemy.hpp"
#include "utils.hpp"

Enemy::Enemy() :
    is_fast_(false)
{}

void
Enemy::update(sf::Time elapsed)
{
    if (is_dead()) {
        return;
    }

    auto direction = util::direction(destination_ - get_position());
    set_velocity(direction * (is_fast_ ? FAST_SPEED : SLOW_SPEED));

    float dt = elapsed.asSeconds();
    move(get_velocity() * dt);
}

void
Enemy::render()
{
    clear_renderings();

    graphic_.setSize(get_dimensions());
    graphic_.setOrigin(get_extents());
    graphic_.setPosition(util::pixelate(get_position()));
    graphic_.setFillColor(sf::Color::Green);
    graphic_.setOutlineThickness(-1.f);
    graphic_.setOutlineColor(sf::Color::Black);

    add_rendering(&graphic_);

    add_renderings(Healthy::render(get_position(), -(get_extents().y + 5.f)));
}

void
Enemy::on_death()
{
    set_alive(false);
}
