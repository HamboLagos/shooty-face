#pragma once

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/System/Clock.hpp>

#include "projectile.hpp"

/** \brief Bullet is a Automatic Projectile which fires in a straight line (like an uzi). */
class Bullet : public Ammunition, public Projectile
{
friend class TestableBullet;

static constexpr float BULLET_SPEED = 750.f;
static constexpr float FIRE_RATE = 5.f; ///< Bullets/Second

public:
    /** \brief Construct a Bullet.
     *
     * Bullets are fired in a linear trajectory, from their current position along the trajectory
     * which passes through its target. */
    Bullet();
    virtual ~Bullet() = default;

    void set_speed(float speed) { speed_ = speed; }

    Projectile* create_projectile() override;
    void reload() override;

    /** \brief Fire this Bullet.
     *
     * On creation, bullets are dead, they are only animated after a call to fire(). */
    void fire() override;

    void update(sf::Time elapsed) override;
    void render() override;

private:
    sf::CircleShape graphic_;

    float speed_;

    sf::Clock firing_pin_;
    bool is_firing_;
};
