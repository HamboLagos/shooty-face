#pragma once

#include "components/AI.hpp"
#include "enemy.hpp"

/** \brief AI control for the enemy class.
 *
 * Moves towards the player until it is hugging them. Later on, we'll teach it how to deal damage
 * and shoot the player, and other nefarious things. */
class AIEnemy : public AI
{
public:
    AIEnemy(Enemy& enemy) :
        AI(enemy)
    { }
    virtual ~AIEnemy() = default;

    void update(float dt) override;

    inline Enemy& get_enemy() { return static_cast<Enemy&>(get_entity()); }
};
