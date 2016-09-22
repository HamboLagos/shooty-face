#pragma once

#include <memory>

#include "entity.hpp"
#include "projectile.hpp"
#include "components/graphics.hpp"

/** \brief Gun is a composite class of projectiles, which uses a set Ammunition factory. */
class Gun : public Entity, public Renderer
{
public:
    using Magazine = std::vector<std::unique_ptr<Projectile>>;

    Gun(const Entity& the_operator);
    virtual ~Gun() = default;

    /** \brief Set the ammunition type for this gun. */
    inline void set_ammunition(std::unique_ptr<Ammunition> ammo) { ammunition_ = std::move(ammo); }

    /** \brief Fire a projectile of the set ammunition type at the given target. */
    void fire(sf::Vector2f target);

    /** \brief Delegates to Ammunition::reload(). */
    void reload();

    void update(sf::Time elapsed) override;
    const Graphics::Renderings render() override;

private:
    const Entity& operator_; ///< The entity operating this gun

    std::unique_ptr<Ammunition> ammunition_; ///< Ammunition type used for next fire()

    Magazine magazine_; ///< The projectiles this gun tracks

    // Test Methods
    friend class TestableGun;
};
