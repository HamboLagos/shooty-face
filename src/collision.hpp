#pragma once

#include <SFML/System/Vector2.hpp>

#include "AABB.hpp"
#include "entity.hpp"

class Collision
{
public:
    Collision() = delete;

    /** \brief Performs sanity check on the two entities.
     *
     * Returns false under the following conditions:
     * 1. First and Second are the same entity (address equality).
     * 2. Either first and/or second has no physics component.
     * 3. Either first and/or second is not a solid.
     *
     * \return true iff none of the above conditions are met. */
    static bool sanity_check(const Entity& first, const Entity& second);

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
     * Results are invalid if broad_test() does not also return true. Assumptions are made in the
     * implementation of narrow_test() that broad_test() signals a potential collision.
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

private:
    static const sf::Vector2f ORIGIN; ///< Origin is <0, 0>
};
