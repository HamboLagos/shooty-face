#pragma once

#include <utility>

#include "AABB.hpp"

class Collision
{
    static const sf::Vector2f ORIGIN;

public:
    Collision() = delete;

    /** \brief Broad phase test for collision between two Entities.
     *
     * Uses the state space for each AABB to determine if a collision may occur. May give a false
     * positive, but is guaranteed to not give a false negative.
     *
     * \return true iff collision may occur. */
    static bool broad_test(const AABB& first, const AABB& second);

    /** \brief Narrow phase test for collision between two Entities.
     *
     * Returns the percentage of the relative trajectory (one.trajectory - other.trajectory)
     * traveled before a collision would occur, in range (0.f, 1.f], or 1.f if no collision will
     * occur.
     *
     * \return Percentage of trajectory until collision (0.f, 1.f], or 1.f if no collision. */
    static float narrow_test(const AABB& first, const AABB& second);

    /** \brief Get the penetration vector for the last positive narrow_test() which returned 0.f.
     *
     * The penetration vector is the minimum distance that second should move to fully
     * un-collide with first. It only has meaning in the case where the initial positions of each
     * AABB are already colliding (narrow_test() returns 0.f);
     *
     * \return Penetration of "first" into "second" iff narrow_test() returns 0.f. */
    static sf::Vector2f get_penetration(const AABB& first, const AABB& second);
};
