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
     * \param[in] position Geometrically cenetered position.
     * \param[in] dimensions <Width, Height> of the AABB. */
    AABB(sf::Vector2f position, sf::Vector2f dimensions) :
        position_(position),
        dimensions_(dimensions)
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

    inline bool contains_point(sf::Vector2f point) const
    {
        sf::Vector2f min = position_ - get_extents();
        sf::Vector2f max = position_ + get_extents();

        return
            min.x < point.x &&
            max.x > point.x &&
            min.y < point.y &&
            max.y > point.y;
    }

    inline bool operator==(const AABB& other) const
    {
        return
            position_   == other.position_ &&
            dimensions_ == other.dimensions_;
    }

    inline bool operator!=(const AABB& other) const
    {
        return !(*this == other);
    }

private:
    sf::Vector2f position_;
    sf::Vector2f dimensions_;
};
