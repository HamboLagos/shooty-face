#pragma once

#include <utility>

#include "AABB.hpp"

class Collision
{
public:
    Collision() :
        penetration_vector_({0.f, 0.f})
    { }

    /** \brief Test for collision between two static entitites.
     *
     * \return true iff collision detected. */
    bool test(AABB one, AABB other);

    /** \brief Get the penetration vector for the most recent collision test.
     *
     * Returns <0, 0> if the most recent collision test was negative. */
    sf::Vector2f get_penetration();

private:
    sf::Vector2f penetration_vector_;
};
