#include "health.hpp"

Health::Health(float base_health) :
    base_health_(base_health),
    health_(base_health)
{
    /* health_bar_.set_dimensions({50.f, 10.f}); */
}

void
Health::set_health(float health)
{
    health_ = std::max(0.f, std::min(base_health_, health));
}

/* Graphical::Renderings */
/* Health::render(sf::Vector2f position, float y_off) */
/* { */
/*     health_bar_.set_filled(health_ / base_health_); */
/*     health_bar_.set_position({position.x, position.y + y_off}); */
/*     health_bar_.render(); */
/*     return health_bar_.get_renderings(); */
/* } */

void
Health::damage(float damage)
{
    health_ = std::max(0.f, std::min(base_health_, health_ - damage));

    if (on_damage_) {
        on_damage_(damage);
    }

    if (on_death_ && health_ == 0.f) {
        on_death_();
    }
}

void
Health::heal(float heal)
{
    health_ = std::max(0.f, std::min(base_health_, health_ + heal));

    if (on_heal_) {
        on_heal_(heal);
    }
}
