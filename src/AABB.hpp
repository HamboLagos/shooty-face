#pragma once

#include <SFML/System/Vector2.hpp>

/** \brief Abstraction for an Axis Aligned Bounding Box.
 *
 * AABB's are defined with:
 * 1. A position determining its geometric center.
 * 2. An X and Y dimension, for width and height, respectively.
 * 3. A trajectory, <dx, dy>, for the current frame. */
class AABB
{
public:
    /** \brief Create a new AABB.
     *
     * For static entities, let trajectory = {0.f, 0.f}.
     *
     * \param[in] position Geometrically cenetered position.
     * \param[in] dimensions <Width, Height> of the AABB.
     * \param[in] trajectory AABB trajectory for this frame (velocity * dt) */
    AABB(sf::Vector2f position, sf::Vector2f dimensions, sf::Vector2f trajectory = {0.f, 0.f});
    ~AABB() = default;

    inline sf::Vector2f get_position() const { return position_; }
    inline sf::Vector2f get_dimensions() const { return dimensions_; }
    inline sf::Vector2f get_extents() const { return dimensions_/2.f; }
    inline sf::Vector2f get_trajectory() const { return trajectory_; }

    /** \brief Returns the corner with the most negative coordinates.
     *
     * For SFML coordinate axis, this is the highest corner in the window, to the most left. */
    sf::Vector2f get_min_corner() const;

    /** \brief Returns the corner with the most positive coordinates.
     *
     * For SFML coordinate axis, this is the lowest corner in the window, to the most right. */
    sf::Vector2f get_max_corner() const;

    /** \brief Returns the corner closest to the origin.
     *
     * If more than one corner is equidistant to the origin, returns get_min_corner(). */
    sf::Vector2f get_near_corner() const;

    /** \brief Returns the corner furthest from the origin.
     *
     * If more than one corner is equidistant to the origin, returns get_max_corner(). */
    sf::Vector2f get_far_corner() const; ///< \return corner furthest from origin, or max corner

    /** \brief Tests whether the given point is contained within this AABB without applying
     * trajectory.
     *
     * To test broadly whether a point could be contained by this AABB, repeat this test with the
     * state space for this AABB, via AABB::state_space_for(this).contains_point(point). */
    bool contains_point(sf::Vector2f point) const;

    inline bool operator==(const AABB& other) const
    {
        return
            position_   == other.position_   &&
            dimensions_ == other.dimensions_ &&
            trajectory_ == other.trajectory_;
    }

    inline bool operator!=(const AABB& other) const
    {
        return !(*this == other);
    }

    /** \brief Returns the state space for the given AABB.
     *
     * The state space is defined as the smallest AABB which fully encompasses all possible
     * positions of the given AABB for the current frame (ie. taking its trajectory into account).
     * The resultant state-space AABB will, by definition, have zero trajectory.
     *
     * For an AABB with a zero trajectory, the state space is a copy of itself. For an AABB with
     * nonzero trajectory, the state space is the AABB with its width and height extended by the
     * trajectory.x and trajectory.y components, respectivly, and its position moved half the
     * distance along trajectory vector. */
    static AABB state_space_for(const AABB& box);

    /** \brief Calculates the minkowski difference of (first - second).
     *
     * The Minkowski difference is a new AABB whose position is first.position - second.position,
     * and dimensions are first.dimensions + second.dimensions.
     *
     * The trajectory of the difference is the relative motion of first WRT second. If first and
     * second are converging, it will point from the new position near to the origin. If they are
     * diverging, it will point far from the origin. */
    static AABB minkowski_difference(const AABB& first, const AABB& second);

private:
    sf::Vector2f position_;  ///< This AABB's position, geometrically centered
    sf::Vector2f dimensions_; ///< This AABB's dimensions, <width, height>
    sf::Vector2f trajectory_; ///< This AABB trajectory, aka travel distance this frame
};
