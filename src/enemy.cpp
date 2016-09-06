#include "enemy.hpp"
#include "utils.hpp"

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
    graphic_.setPosition(pixelate(get_position()));
    graphic_.setFillColor(sf::Color::Green);

    return graphic_;
}

void
Enemy::on_death()
{
    static bool trigger_respawn = false;

    kill();

    if (!trigger_respawn) {
        trigger_respawn = true;
    } else {
        trigger_respawn = false;
        set_health(100.f);
        animate();
    }
}
