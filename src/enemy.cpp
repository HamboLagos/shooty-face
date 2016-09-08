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

void
Enemy::render()
{
    clear_renderings();

    graphic_.setSize(get_dimensions());
    graphic_.setOrigin(get_extents());
    graphic_.setPosition(pixelate(get_position()));
    graphic_.setFillColor(sf::Color::Green);
    add_rendering(&graphic_);

    add_renderings(Healthy::render(get_position(), -(get_extents().y + 5.f)));
}

void
Enemy::on_death()
{
    static bool trigger_respawn = false;

    set_alive(false);

    if (!trigger_respawn) {
        trigger_respawn = true;
    } else {
        trigger_respawn = false;
        set_health(100.f);
        set_alive(true);
    }
}
