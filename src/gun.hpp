#pragma once

#include <memory>

#include "entity.hpp"
#include "projectile.hpp"
#include "graphics.hpp"

/** \brief Gun is a composite class of projectiles, which uses a set Ammunition factory. */
class Gun
{
public:
    using Magazine = std::vector<std::unique_ptr<Projectile>>;

    Gun(const Entity& the_operator);
    virtual ~Gun() = default;

    /** \brief Set the ammunition type for this gun. */
    inline void set_ammunition(std::unique_ptr<Ammunition> ammo) { ammunition_ = std::move(ammo); }

    /** \brief Test Method, returns the ammunition loaded in this gun. */
    inline Ammunition* get_ammunition() const { return ammunition_.get(); }

    /** \brief Returns the live projectiles tracked by this gun. */
    inline Magazine& get_magazine() { return magazine_; }

    /** \brief Fire a projectile of the set ammunition type at the given target.
     *
     * Automatic vs. Semiautomatic operation is determined by the ammunition type. */
    void fire(sf::Vector2f target);

    /** \brief Prepare for the next volley.
     *
     * Automatic vs. Semiautomatic operation is determined by the ammunition type. */
    void reload();

    /** \brief Update this gun.
     *
     * Delegates updates to each of the live projectiles. */
    void update(sf::Time elapsed);

    /** \brief Render this gun.
     *
     * Delegates and collects the renderings from each of the live projectiles. */
    const Graphics::Renderings render();

private:
    const Entity& operator_; ///< The entity operating this gun

    std::unique_ptr<Ammunition> ammunition_; ///< Ammunition type used for next fire()

    Magazine magazine_; ///< The projectiles this gun tracks
};
