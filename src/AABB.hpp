#pragma once

#include <SFML/Graphics/Transformable.hpp>

/** \brief Abstraction for an Axis Aligned Bounding Box.
 *
 * AABB's are defined with:
 * 1. An origin at its geometric center.
 * 2. An X and Y dimension, for width and height, respectively. */
class AABB
{
public:
    AABB(sf::Vector2f origin, sf::Vector2f dimensions) :
        origin_(origin),
        dimensions_(dimensions)
    { }

    inline sf::Vector2f get_origin() const { return origin_; }
    inline float get_x_origin() const { return origin_.x; }
    inline float get_y_origin() const { return origin_.y; }

    inline sf::Vector2f get_dimensions() const { return dimensions_; }
    inline float get_x_dimension() const { return dimensions_.x; }
    inline float get_y_dimension() const { return dimensions_.y; }

    inline sf::Vector2f get_extents() const { return dimensions_/2.f; }
    inline float get_x_extent() const { return dimensions_.x/2.f; }
    inline float get_y_extent() const { return dimensions_.y/2.f; }

    inline bool contains_point(sf::Vector2f point)
    {
        sf::Vector2f min = origin_ - get_extents();
        sf::Vector2f max = origin_ + get_extents();

        return
            min.x < point.x &&
            max.x > point.x &&
            min.y < point.y &&
            max.y > point.y;
    }

    inline bool operator==(const AABB& other)
    {
        return
            origin_     == other.origin_ &&
            dimensions_ == other.dimensions_;
    }

    inline bool operator!=(const AABB& other)
    {
        return !(*this == other);
    }

private:
    sf::Vector2f origin_;
    sf::Vector2f dimensions_;
};
