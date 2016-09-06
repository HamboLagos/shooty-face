#include "healthy.hpp"

Healthy::Healthy(float intial_health) :
    health_(intial_health)
{ }

void
Healthy::damage(float damage)
{
    health_ -= damage;

    on_damage(damage);
    if (health_ <= 0.f) {
        on_death();
    }
}

void
Healthy::heal(float heal)
{
    health_ += heal;

    on_heal(heal);
}
