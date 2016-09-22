#pragma once

#include "components/AI.hpp"
#include "bullet.hpp"

/** \brief AI Control for the Bullet Class.
 *
 * Moves the position along the velocity trajectory until it hits an impassable entity. Calls back
 * to the owner bullet to apply damage, */
class AIBullet : public AI
{
public:
    AIBullet(Bullet& bullet) :
        AI(bullet)
    { }
    virtual ~AIBullet() = default;

    sf::Time update(sf::Time elapsed) override;

    inline Bullet& get_bullet() { return static_cast<Bullet&>(get_entity()); }
};
