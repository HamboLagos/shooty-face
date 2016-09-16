#pragma once

#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/CircleShape.hpp>

#include "projectile.hpp"

/** \brief Bullet is a automatic Projectile which fires in a straight line (like an uzi). */
class Bullet : public Projectile, public Ammunition
{
friend class TestableBullet;

static constexpr float SPEED = 750.f;
static constexpr float FIRE_RATE = 5.f; ///< Bullets/Second

public:
    Bullet();

    virtual ~Bullet() = default;

    Projectile* create_projectile() override;
    void reload() override;
    void fire() override;
    void update(sf::Time elapsed) override;
    const sf::Drawable* render() override;

private:
    bool is_firing_;       ///< With firing_pin_, limits the firing rate
    sf::Clock firing_pin_; ///< with is_firing_, Limits the firing rate

    sf::CircleShape graphic_;
};
