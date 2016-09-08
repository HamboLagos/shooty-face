#include "healthy.hpp"

Healthy::Healthy(float base_health) :
    base_health_(base_health),
    health_(base_health)
{
    health_bar_.set_dimensions({50.f, 10.f});
}

void
Healthy::set_health(float health)
{
    health_ = std::max(0.f, std::min(base_health_, health));
}

Graphical::Renderings
Healthy::render(sf::Vector2f position, float y_off)
{
    health_bar_.set_filled(health_ / base_health_);
    health_bar_.set_position({position.x, position.y + y_off});
    health_bar_.render();
    return health_bar_.get_renderings();
}

void
Healthy::damage(float damage)
{
    health_ = std::max(0.f, std::min(base_health_, health_ - damage));

    on_damage(damage);
    if (health_ <= 0.f) {
        on_death();
    }
}

void
Healthy::heal(float heal)
{
    health_ = std::max(0.f, std::min(base_health_, health_ + heal));

    on_heal(heal);
}
