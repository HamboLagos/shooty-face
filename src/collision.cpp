#include <cmath>

#include "collision.hpp"

AABB minkowski_difference(AABB one, AABB other);
sf::Vector2f get_penetration_vector(AABB mk_diff, sf::Vector2f point);

bool
Collision::test(AABB one, AABB other)
{
    // dual static entities collision detection
    AABB mk_diff = minkowski_difference(one, other);
    sf::Vector2f origin = {0.f, 0.f};
    bool positive = mk_diff.contains_point(origin);

    if (!positive) {
        penetration_vector_ = {0.f, 0.f};
    } else {
        penetration_vector_ = -get_penetration_vector(mk_diff, origin);
    }

    return positive;
}

sf::Vector2f
Collision::get_penetration()
{
    return penetration_vector_;
}

AABB
minkowski_difference(AABB one, AABB other)
{
    sf::Vector2f one_min = one.get_position() - one.get_extents();
    sf::Vector2f other_max = other.get_position() + other.get_extents();

    sf::Vector2f new_min = one_min - other_max;
    sf::Vector2f new_dimensions = one.get_dimensions() + other.get_dimensions();
    sf::Vector2f new_position = new_min + new_dimensions/2.f;

    return AABB(new_position, new_dimensions);
}

sf::Vector2f
get_penetration_vector(AABB mk_diff, sf::Vector2f point)
{
    sf::Vector2f min = mk_diff.get_position() - mk_diff.get_extents();
    sf::Vector2f max = mk_diff.get_position() + mk_diff.get_extents();

    float min_distance = std::abs(point.x - min.x);
    sf::Vector2f ret = {min.x, point.y};

    float temp = std::abs(point.x - max.x);
    if (temp < min_distance) {
        min_distance = temp;
        ret = {max.x, point.y};
    }

    temp = std::abs(point.y - min.y);
    if (temp < min_distance) {
        min_distance = temp;
        ret = {point.x, min.y};
    }

    temp = std::abs(point.x - max.y);
    if (temp < min_distance) {
        min_distance = temp;
        ret = {point.x, max.y};
    }

    return ret;
}
