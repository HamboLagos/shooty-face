#pragma once

#include <memory>

#include "entity.hpp"

class Projectile : public GraphicalEntity
{
public:
    Projectile() = default;
    virtual ~Projectile() = default;

    void set_target(sf::Vector2f target) { target_ = target; }
    sf::Vector2f get_target() const { return target_; }

    /** \brief Fires the projectile.
     *
     * Clients should set a target for the projectile first, via set_target().*/
    virtual void fire() = 0;

private:
    sf::Vector2f target_;
};

/** \brief Factory Class for projectiles. */
class Ammunition
{
public:
    /** \brief Creates a new projectile.
     *
     * Callee assumes responsiblity of the returned projectile. We can't return a std::unique_ptr
     * due to a limitation of GoogleMock requiring return values to be copy-assignable. */
    virtual Projectile* create_projectile() = 0;
};
