#include <cmath>

#include "collision.hpp"
#include "utils.hpp"

const sf::Vector2f Collision::ORIGIN = sf::Vector2f(0.f, 0.f);

bool
Collision::broad_test(const AABB& first, const AABB& second)
{
    auto mk_diff = AABB::minkowski_difference(AABB::state_space_for(first),
                                              AABB::state_space_for(second));
    return mk_diff.contains_point(ORIGIN);
}

float
Collision::narrow_test(const AABB& first, const AABB& second)
{
    // protect ourselves from impossible collisions
    if (!broad_test(first, second)) {
        return 1.f;
    }

    auto mk_diff = AABB::minkowski_difference(first, second);

    // colliding before trajectory applied
    if (mk_diff.contains_point(ORIGIN)) {
        return 0.f;
    }

    // Find the entry and exit time for x and y
    // using [noonat.github.io/intersect]
    auto near = mk_diff.get_near_corner();
    auto far = mk_diff.get_far_corner();

    float entry_x = near.x / -mk_diff.get_x_trajectory();
    float exit_x  = far.x  / -mk_diff.get_x_trajectory();
    float entry_y = near.y / -mk_diff.get_y_trajectory();
    float exit_y  = far.y  / -mk_diff.get_y_trajectory();

    if (mk_diff.get_x_trajectory() == 0.f) {
        entry_x = -std::numeric_limits<float>::infinity();
        exit_x = std::numeric_limits<float>::infinity();
    }

    if (mk_diff.get_y_trajectory() == 0.f) {
        entry_y = -std::numeric_limits<float>::infinity();
        exit_y = std::numeric_limits<float>::infinity();
    }

    if (entry_x > exit_y || entry_y > exit_x) {
        return 1.f;
    }

    float entry = std::max(entry_x, entry_y);
    float exit = std::min(exit_x, exit_y);

    if (entry >= 1.f || exit <= 0.f) {
        return 1.f;
    }

    return entry;
}

sf::Vector2f
Collision::get_penetration(const AABB& first, const AABB& second)
{
    auto mk_diff = AABB::minkowski_difference(first, second);
    auto near = mk_diff.get_near_corner();

    // if it's rigt on the edge, return a small value
    float small_value = 0.01f;
    if (near.x == 0.f) {
        return -util::direction({mk_diff.get_far_corner().x, 0.f}) * small_value;
    }
    if (near.y == 0.f) {
        return -util::direction({0.f, mk_diff.get_far_corner().y}) * small_value;
    }

    // if penetration is nonzero, return it
    if (std::abs(near.x) <= std::abs(near.y)) {
        return sf::Vector2f(near.x, 0.f);
    }

    return sf::Vector2f(0.f, near.y);
}
