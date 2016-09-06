#pragma once

#include <SFML/Graphics/CircleShape.hpp>

#include "projectile.hpp"

class Bullet : public Projectile
{
friend class TestableBullet;
public:
    /** \brief Create a new bullet ready to be fired.
     *
     * Bullets are fired in a linear trajectory, beginning at its initial position, and traversing
     * along the trajectory vector which passes through its target.
     *
     * \param[in] initial_position The starting position of this Bullet.
     * \param[in] target Where to aim this Bullet.
     * \param[in] speed Magnitude of this Bullet's velocity. */
    Bullet(sf::Vector2f initial_position, sf::Vector2f target, float speed);
    ~Bullet() override = default;

    /** \brief Fire this Bullet.
     *
     * On creation, bullets are "dead", they only animated after a call to fire(). */
    void fire() override;

    void update(sf::Time elapsed) override;
    const sf::Drawable& render() override;

private:
    sf::CircleShape graphic_;
};
