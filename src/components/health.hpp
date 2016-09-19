#pragma once

#include "entity.hpp"

class Health : public Component
{
public:
    Health(Entity& entity, float base_health = 100.f) :
        Component(entity),
        base_health_(base_health),
        health_(base_health)
    { }
    virtual ~Health() = default;

    /** \brief Kills the entity if health has dropped to zero. */
    inline void update()
    {
        if (health_ == 0.f) {
            get_entity().kill();
        }
    }

    /** \brief Set health to an absolute value.
     *
     * health is coerced to nearest value in range [0.f, base_health]. */
    inline void set_health(float health)
    {
        health_ = std::max(0.f, std::min(health, base_health_));
        update();
    }

    inline float get_health() const { return health_; };

    /** \brief Reduces health by the given amount.
     *
     * health is coerced to nearest value in range [0.f, base_health]. */
    inline void damage(float damage) { set_health(health_ - damage); }

    /** \brief Increases health by the given amount.
     *
     * health is coerced to nearest value in range [0.f, base_health]. */
    inline void heal(float heal) { set_health(health_ + heal); }

private:
    float base_health_;
    float health_;
};
