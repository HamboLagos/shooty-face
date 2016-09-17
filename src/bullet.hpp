#pragma once

#include <SFML/Graphics/CircleShape.hpp>

#include "clock.hpp"
#include "projectile.hpp"
#include "components/graphics.hpp"

/** \brief Bullet is a automatic Projectile which fires in a straight line (like an uzi). */
class Bullet : public Projectile, public Renderer
{
public:
    static constexpr float SPEED = 750.f; ///< Bullet flight speed
    static constexpr float DAMAGE = 5.f;  ///< Bullet Damage on impact

    Bullet();
    virtual ~Bullet() = default;

    void fire() override;
    void update(sf::Time elapsed) override;
    const Renderings render() override;

private:
    sf::CircleShape graphic_;
};

/** \brief Ammunition Factory for bullets.
 *
 * Limits the firing of bullets. */
class BulletAmmunition : public Ammunition
{
public:
    static constexpr float FIRE_RATE = 5.f; ///< Bullets/Second

    BulletAmmunition() :
        BulletAmmunition(std::make_unique<Clock>())
    { }
    virtual ~BulletAmmunition() = default;

    Projectile* create_projectile() override;
    void reload() override;

private:
    bool is_firing_; ///< With firing_pin_, limits the firing rate
    std::unique_ptr<ClockIF> firing_pin_; ///< with is_firing_, limits the firing rate

/// Test Methods
private:
    friend class TestableBulletAmmunition;

    BulletAmmunition(std::unique_ptr<ClockIF> clock) :
        firing_pin_(std::move(clock)),
        is_firing_(false)
    { }
};
