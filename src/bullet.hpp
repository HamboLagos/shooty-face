#pragma once

#include <SFML/Graphics/CircleShape.hpp>

#include "projectile.hpp"

/** \brief Bullet is a Projectile which fires in a straight line. */
class Bullet : public Ammunition, public Projectile
{
friend class TestableBullet;

static constexpr float BULLET_SPEED = 250.f;

public:
    /** \brief Construct a Bullet.
     *
     * Bullets are fired in a linear trajectory, from their current position along the trajectory
     * which passes through its target. */
    Bullet();
    virtual ~Bullet() = default;

    void set_speed(float speed) { speed_ = speed; }

    Projectile* create_projectile() override;

    /** \brief Fire this Bullet.
     *
     * On creation, bullets are dead, they only animated after a call to fire(). */
    void fire() override;

    void update(sf::Time elapsed) override;
    void render() override;

private:
    sf::CircleShape graphic_;

    float speed_;
};
