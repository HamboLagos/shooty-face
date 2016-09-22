#pragma once

#include "entity.hpp"
#include "components/graphics.hpp"

/** \brief Base class for anything which can be shot from a gun. */
class Projectile : public Entity, public Renderer
{
public:
    Projectile() = default;
    virtual ~Projectile() = default;

    /** \brief Set the target for this projectile.
     *
     * The semantics of projectile trajectory depend on the specific instance of Projectile type. */
    inline void set_target(sf::Vector2f target) { target_ = target; }

    /** \brief Returns the target for this projectile. */
    inline sf::Vector2f get_target() const { return target_; }

    /** \brief Fires the projectile.
     *
     * Clients should set a target for the projectile first, via set_target().*/
    virtual void fire() = 0;
    virtual void update(sf::Time elapsed) = 0;
    virtual const Renderings render() = 0;

private:
    sf::Vector2f target_; ///< The target of this projectile
};

/** \brief Factory Class for projectiles.
 *
 * Assignable as ammunition for the Gun class. */
class Ammunition
{
public:
    Ammunition() = default;
    virtual ~Ammunition() = default;

    /** \brief Creates a new projectile.
     *
     * Callee assumes responsiblity of the returned projectile. We can't return a std::unique_ptr
     * due to a limitation of GoogleMock requiring return values to be copy-assignable. This sucks,
     * but the workarounds are kinda ugly, so we'll live with it. */
    virtual Projectile* create_projectile() = 0;

    /** \brief Prepares this ammunition for the next volley.
     *
     * When to reload is left up to client discretion, as it may vary by ammunition type. */
    virtual void reload() = 0;
};
