#pragma once

#include "entity.hpp"

class Projectile : public Entity
{
public:
    Projectile() = default;
    virtual ~Projectile() = default;

    /** \brief Fires the projectile.
     *
     * This is typically called from the gun interface, but is left as part of the public interface
     * during development to ease rapid prototyping of new bullet types. The semantics of firing
     * a projectile is contextually dependent on the bullet type, refer to its documentation for
     * explicit details. */
    virtual void fire() = 0;
};
