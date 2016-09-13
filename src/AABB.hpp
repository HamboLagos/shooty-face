#pragma once

#include <SFML/Graphics/Transformable.hpp>

/** \brief Abstraction for an Axis Aligned Bounding Box.
 *
 * AABB's are defined with:
 * 1. A position determining its geometric center.
 * 2. An X and Y dimension, for width and height, respectively. */
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
    AABB(sf::Vector2f position, sf::Vector2f dimensions, sf::Vector2f trajectory = {0.f, 0.f}) :
        position_(position),
        dimensions_(dimensions),
        trajectory_(trajectory)
    { }

    inline sf::Vector2f get_position() const { return position_; }
    inline float get_x_position() const { return position_.x; }
    inline float get_y_position() const { return position_.y; }

    inline sf::Vector2f get_dimensions() const { return dimensions_; }
    inline float get_x_dimension() const { return dimensions_.x; }
    inline float get_y_dimension() const { return dimensions_.y; }

    inline sf::Vector2f get_extents() const { return dimensions_/2.f; }
    inline float get_x_extent() const { return dimensions_.x/2.f; }
    inline float get_y_extent() const { return dimensions_.y/2.f; }

    inline sf::Vector2f get_trajectory() const { return trajectory_; }
    inline float get_x_trajectory() const { return trajectory_.x; }
    inline float get_y_trajectory() const { return trajectory_.y; }

    sf::Vector2f get_min_corner() const; ///< \return corner with most negative coordinates
    sf::Vector2f get_max_corner() const; ///< \return corner with most positive coordinates
    sf::Vector2f get_near_corner() const; ///< \return corner closest to origin, or min corner
    sf::Vector2f get_far_corner() const; ///< \return corner furthest from origin, or max corner

    inline bool contains_point(sf::Vector2f point) const
    {
        auto min = get_min_corner();
        auto max = get_max_corner();

        return
            min.x < point.x &&
            max.x > point.x &&
            min.y < point.y &&
            max.y > point.y;
    }

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
     * positions of the given AABB for the current frame (ie. given its initial position and
     * trajectory). For an AABB with a zero trajectory, the state space is itself the AABB. */
    static AABB state_space_for(const AABB& box);

    /** \brief Calculates the minkowski difference (first - second).
     *
     * Resultant trajectory of the difference is the relative motion of first WRT second. If first
     * and second are converging, it will point near the origin. */
    static AABB minkowski_difference(const AABB& first, const AABB& second);

private:
    sf::Vector2f position_;
    sf::Vector2f dimensions_;
    sf::Vector2f trajectory_;
};
